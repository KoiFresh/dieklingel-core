import 'dart:async';
import 'dart:convert';

import 'package:flutter/foundation.dart';
import 'package:mqtt/models/mqtt_uri.dart';
import 'package:mqtt/models/request.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'package:path/path.dart' as path;

import 'factories/mqtt_client_factory.dart';
import 'client_interface.dart';
import 'models/connection_state.dart';
import 'models/legacy_mqtt_response.dart';
import 'models/message.dart';
import 'models/response.dart';

class Client implements IClient {
  final MqttClientFactory factory;
  final Map<String, StreamController<Message>> _subscribtions = {};
  MqttClient? _client;
  String? _channel;

  Client({@visibleForTesting this.factory = const MqttClientFactory()});

  String? get prefix => _channel;

  @override
  void answer(
    String channel,
    Future<Response> Function(Request request) handler,
  ) {
    watch(channel).listen((event) async {
      Request req = Request.fromMap(jsonDecode(event.payload));
      Response response = await handler(req);
      Message message = Message(
        path.normalize("${event.topic}/${req.location.path}"),
        jsonEncode(response.toMap()),
      );

      publish(message);
    });
  }

  @override
  void legacyAnswer(
    String channel,
    Future<LegacyMqttResponse> Function(String request) handler,
  ) {
    watch(channel).listen(
      (event) async {
        LegacyMqttResponse response = await handler(event.payload);
        publish(
          Message(
            "${event.topic}/response",
            jsonEncode(response.toMap()),
          ),
        );
      },
    );
  }

  @override
  Future<void> connect(
    MqttUri uri, {
    String? username,
    String? password,
    String? identifier,
  }) async {
    await disconnect();

    _client = factory.create(uri.toHost(), identifier ?? "")
      ..port = uri.port
      ..keepAlivePeriod = 20
      ..setProtocolV311()
      ..autoReconnect = true;

    _channel = uri.channel;

    await _client?.connect(username, password);

    _client?.updates?.listen((event) {
      MqttPublishMessage rec = event[0].payload as MqttPublishMessage;
      final String topic = event[0].topic;
      List<int> messageAsBytes = rec.payload.message;
      String message = utf8.decode(messageAsBytes);

      for (final subscription in _subscribtions.entries) {
        String channel = subscription.key;

        RegExp regex = RegExp(
          "^${channel.replaceAll("/+", "/[^/]+").replaceAll("#", ".+")}\$",
        );

        String localTopic = MqttUri(
          channel: topic.replaceFirst(_channel ?? "", ""),
        ).channel;

        if (!regex.hasMatch(localTopic)) {
          continue;
        }

        subscription.value.add(
          Message(
            topic.replaceFirst(_channel ?? "", ""),
            message,
          ),
        );
      }
    });

    _client?.subscribe(
      MqttUri(channel: "$_channel/#").channel,
      MqttQos.exactlyOnce,
    );
  }

  @override
  Future<void> disconnect() async {
    _client?.disconnect();
    _client = null;
  }

  @override
  Future<Response> request(
    Request request, {
    Duration timeout = const Duration(seconds: 30),
  }) async {
    Completer<String?> completer = Completer<String?>();

    StreamSubscription subscription = watch(request.location.path).listen(
      (event) {
        completer.complete(event.payload);
      },
    );

    Message message = Message(
      request.location.path,
      jsonEncode(request.body),
    );

    publish(message);
    String? result = await completer.future.timeout(
      timeout,
      onTimeout: () => null,
    );

    await subscription.cancel();

    Response response;

    if (result == null) {
      return const Response(status: -1, message: "timeout");
    }

    try {
      response = Response.fromMap(jsonDecode(result));
    } catch (exception) {
      return Response(status: -1, message: exception.toString());
    }

    return response;
  }

  @override
  ConnectionState get state {
    return _client?.connectionStatus?.state == MqttConnectionState.connected
        ? ConnectionState.connected
        : ConnectionState.disconnected;
  }

  @override
  Stream<Message> watch(String channel) {
    StreamController<Message> controller = _subscribtions.putIfAbsent(
      channel,
      () => StreamController.broadcast(),
    );

    controller.onCancel = () {
      if (!controller.hasListener) {
        _subscribtions.remove(channel);
      }
    };

    return controller.stream;
  }

  @override
  void publish(Message message) {
    if (state != ConnectionState.connected) {
      return;
    }

    MqttClientPayloadBuilder builder = MqttClientPayloadBuilder();
    builder.addUTF8String(message.payload);

    String channel = MqttUri(channel: "$_channel/${message.topic}").channel;

    _client?.publishMessage(
      channel,
      MqttQos.exactlyOnce,
      builder.payload!,
    );
  }
}

extension _ToHost on MqttUri {
  String toHost() {
    String protocol = "";

    if (websocket) {
      protocol = ssl ? "wss://" : "ws://";
    }

    return "$protocol$host";
  }
}
