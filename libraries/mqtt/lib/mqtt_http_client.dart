import 'dart:async';
import 'dart:convert';

import 'package:http/http.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'package:uuid/uuid.dart';
import 'package:path/path.dart' as path;

import 'factories/mqtt_client_factory.dart';

class MqttHttpClient {
  final MqttClientFactory _factory;

  MqttHttpClient({
    MqttClientFactory factory = const MqttClientFactory(),
  }) : _factory = factory;

  Future<Response> _fetch(Request request) async {
    final String hostname =
        request.url.isScheme("ws") || request.url.isScheme("wss")
            ? "${request.url.scheme}://${request.url.host}"
            : request.url.host;

    final MqttClient client = _factory.create(hostname, const Uuid().v4())
      ..port = request.url.port
      ..keepAlivePeriod = 20
      ..setProtocolV311()
      ..autoReconnect = true;

    await client.connect(
      request.headers["username"],
      request.headers["password"],
    );

    if (request.headers["mqtt_answer_channel"] == null) {
      request.headers["mqtt_answer_channel"] = const Uuid().v4();
    }

    Completer<String?> completer = Completer<String?>();

    client.updates?.listen((event) {
      MqttPublishMessage rec = event[0].payload as MqttPublishMessage;
      // final String topic = event[0].topic;
      List<int> messageAsBytes = rec.payload.message;
      String message = utf8.decode(messageAsBytes);
      completer.complete(message);
    });

    String requestPath = path.normalize("./${request.url.path}");
    String answerPath = path.normalize(
      "$requestPath/${request.headers["mqtt_answer_channel"]}",
    );

    client.subscribe(
      answerPath,
      MqttQos.exactlyOnce,
    );

    String payload = jsonEncode(_requestToMap(request));
    client.publishMessage(
      requestPath,
      MqttQos.exactlyOnce,
      MqttClientPayloadBuilder().addUTF8String(payload).payload!,
    );

    String? message = await completer.future.timeout(
      const Duration(seconds: 30),
      onTimeout: () => null,
    );

    if (message == null) {
      throw TimeoutException(
        "There was no response received in the given time",
      );
    }

    Map<String, dynamic> map = jsonDecode(message);
    Map<String, dynamic>? headers = map["headers"];

    return Response(
      map["body"],
      map["statusCode"],
      request: request,
      headers: headers?.cast<String, String>() ?? {},
    );
  }

  Map<String, dynamic> _requestToMap(Request request) {
    return {
      "method": request.method,
      "headers": request.headers,
      "body": request.body,
    };
  }

  Future<Response> _send(
    String method,
    Uri url,
    Map<String, String>? headers, [
    Object? body,
    Encoding? encoding,
  ]) {
    Request request = Request(method, url);

    if (headers != null) {
      request.headers.addAll(headers);
    }
    if (encoding != null) {
      request.encoding = encoding;
    }
    if (body != null) {
      if (body is String) {
        request.body = body;
      } else if (body is List) {
        request.bodyBytes = body.cast<int>();
      } else if (body is Map) {
        request.bodyFields = body.cast<String, String>();
      } else {
        throw ArgumentError('Invalid request body "$body"');
      }
    }

    return _fetch(request);
  }

  Future<Response> get(Uri url, {Map<String, String>? headers}) =>
      _send("GET", url, headers);

  Future<Response> post(
    Uri url, {
    Map<String, String>? headers,
    Object? body,
    Encoding? encoding,
  }) =>
      _send("POST", url, headers, body, encoding);

  Future<Response> put(
    Uri url, {
    Map<String, String>? headers,
    Object? body,
    Encoding? encoding,
  }) =>
      _send("PUT", url, headers, body, encoding);

  Future<Response> patch(
    Uri url, {
    Map<String, String>? headers,
    Object? body,
    Encoding? encoding,
  }) =>
      _send("PATCH", url, headers, body, encoding);

  Future<Response> delete(
    Uri url, {
    Map<String, String>? headers,
    Object? body,
    Encoding? encoding,
  }) =>
      _send("DELETE", url, headers, body, encoding);
}
