import 'dart:async';
import 'dart:convert';

import 'package:mqtt_client/mqtt_client.dart';
import 'package:path/path.dart' as path;
import 'package:shelf/shelf.dart';
import 'package:shelf_router/shelf_router.dart';

import 'factories/mqtt_client_factory.dart';

class MqttHttpServer {
  final MqttClientFactory _factory;

  MqttClient? _client;
  Handler? _handler;

  MqttHttpServer({
    MqttClientFactory factory = const MqttClientFactory(),
  }) : _factory = factory;

  Future<void> serve(
    Handler handler,
    Uri host, {
    String powerdBy = "dieKlingel",
    String? username,
    String? password,
  }) async {
    if (host.path.isEmpty) {
      throw ArgumentError("The path cannot be empty!");
    }

    _client?.disconnect();

    final String hostname = host.isScheme("ws") || host.isScheme("wss")
        ? "${host.scheme}://${host.host}"
        : host.host;

    final String prefix = path.normalize("./${host.path}");

    _client = _factory.create(hostname, powerdBy)
      ..port = host.port
      ..keepAlivePeriod = 20
      ..setProtocolV311()
      ..autoReconnect = true;

    await _client?.connect(username, password);

    _handler = Router()..mount("/$prefix", handler);

    _client?.updates?.listen((event) async {
      MqttPublishMessage rec = event[0].payload as MqttPublishMessage;
      final String topic = event[0].topic;
      List<int> messageAsBytes = rec.payload.message;
      String message = utf8.decode(messageAsBytes);

      Request request;
      try {
        Map<String, dynamic> payload = jsonDecode(message);
        if (payload.containsKey("statusCode")) {
          // if it has a status code it is a answer
          return;
        }

        Map<String, dynamic>? headers = payload["headers"];
        Uri requestedUri = Uri.parse("/$topic");

        request = Request(
          payload["method"] ?? "GET",
          requestedUri.replace(
            host: host.host,
            port: host.port,
            scheme: host.scheme,
          ),
          headers: headers?.cast<String, Object>(),
          url: Uri.parse(topic),
        );
      } on RequestParseError catch (error) {
        print(error.message);
        return;
      } on FormatException catch (error) {
        print(error.message);
        return;
      }

      String? answerChannel = request.headers["mqtt_answer_channel"];
      if (answerChannel == null) {
        print("request is missing mqtt_answer_channel header");
        return;
      }

      Response response = await _handler!(request);
      String answer = jsonEncode(await _responseToMap(response));

      _client?.publishMessage(
        path.normalize("$topic/$answerChannel"),
        MqttQos.exactlyOnce,
        MqttClientPayloadBuilder().addUTF8String(answer).payload!,
      );
    });

    _client?.subscribe(path.normalize("$prefix/#"), MqttQos.exactlyOnce);
  }

  Future<Map<String, dynamic>> _responseToMap(Response response) async {
    return {
      "statusCode": response.statusCode,
      "headers": response.headers,
      "mimeType": response.mimeType,
      "body": await response.readAsString(),
    };
  }
}

class RequestParseError extends Error {
  final String message;

  RequestParseError(this.message);
}
