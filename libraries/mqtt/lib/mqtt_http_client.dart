import 'dart:async';
import 'dart:convert';

import 'package:http/http.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'package:uuid/uuid.dart';
import 'package:path/path.dart' as path;

import 'factories/mqtt_client_factory.dart';

class MqttHttpClient {
  final MqttClientFactory _factory;
  final bool _keepConnectionAlive;
  MqttClient? _client;

  MqttHttpClient({
    MqttClientFactory factory = const MqttClientFactory(),
  })  : _factory = factory,
        _keepConnectionAlive = false;

  MqttHttpClient.keepAlive({
    MqttClientFactory factory = const MqttClientFactory(),
  })  : _factory = factory,
        _keepConnectionAlive = true;

  Future<Response> _fetch(Request request) async {
    final String hostname =
        request.url.isScheme("ws") || request.url.isScheme("wss")
            ? "${request.url.scheme}://${request.url.host}"
            : request.url.host;

    final MqttClient client =
        _client ?? _factory.create(hostname, const Uuid().v4())
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
    request.headers.remove("is_socket_message");

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

    if (!_keepConnectionAlive) {
      client.disconnect();
      _client = null;
    }

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

  Future<void> _socket(Request request) async {
    final String hostname =
        request.url.isScheme("ws") || request.url.isScheme("wss")
            ? "${request.url.scheme}://${request.url.host}"
            : request.url.host;

    final MqttClient client =
        _client ?? _factory.create(hostname, const Uuid().v4())
          ..port = request.url.port
          ..keepAlivePeriod = 20
          ..setProtocolV311()
          ..autoReconnect = true;

    await client.connect(
      request.headers["username"],
      request.headers["password"],
    );

    request.headers["is_socket_message"] = "true";
    request.headers.remove("mqtt_answer_channel");

    String requestPath = path.normalize("./${request.url.path}");

    String payload = jsonEncode(_requestToMap(request));
    client.publishMessage(
      requestPath,
      MqttQos.exactlyOnce,
      MqttClientPayloadBuilder().addUTF8String(payload).payload!,
    );

    if (_keepConnectionAlive) {
      return;
    }
    //client.disconnect();
    //_client = null;
  }

  Map<String, dynamic> _requestToMap(Request request) {
    return {
      "method": request.method,
      "headers": request.headers,
      "body": request.body,
    };
  }

  Future<Request> _createRequest(
    String method,
    Uri url,
    Map<String, String>? headers, [
    Object? body,
    Encoding? encoding,
  ]) async {
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

    return request;
  }

  Future<Response> get(Uri url, {Map<String, String>? headers}) =>
      _createRequest("GET", url, headers).then((req) => _fetch(req));

  Future<Response> post(
    Uri url, {
    Map<String, String>? headers,
    Object? body,
    Encoding? encoding,
  }) =>
      _createRequest("POST", url, headers, body, encoding)
          .then((req) => _fetch(req));

  Future<Response> put(
    Uri url, {
    Map<String, String>? headers,
    Object? body,
    Encoding? encoding,
  }) =>
      _createRequest("PUT", url, headers, body, encoding)
          .then((req) => _fetch(req));

  Future<Response> patch(
    Uri url, {
    Map<String, String>? headers,
    Object? body,
    Encoding? encoding,
  }) =>
      _createRequest("PATCH", url, headers, body, encoding)
          .then((req) => _fetch(req));

  Future<Response> delete(
    Uri url, {
    Map<String, String>? headers,
    Object? body,
    Encoding? encoding,
  }) =>
      _createRequest("DELETE", url, headers, body, encoding)
          .then((req) => _fetch(req));

  Future<void> socket(
    Uri url, {
    Map<String, String>? headers,
    Object? body,
    Encoding? encoding,
  }) =>
      _createRequest("CONNECT", url, headers, body, encoding)
          .then((req) => _socket(req));
}
