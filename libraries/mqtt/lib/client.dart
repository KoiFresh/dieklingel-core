import 'dart:async';
import 'dart:convert';

import 'package:flutter/foundation.dart';
import 'package:mqtt/exceptions/illegal_uri_exception.dart';
import 'package:mqtt_client/mqtt_client.dart';

import 'factories/mqtt_client_factory.dart';
import 'client_interface.dart';
import 'models/connection_state.dart';
import 'models/message.dart';
import 'models/response.dart';

class Client implements ClientInterface {
  final MqttClientFactory factory;
  final Map<String, StreamController<Message>> _subscribtions = {};
  MqttClient? _client;
  String? _channel;

  Client({@visibleForTesting this.factory = const MqttClientFactory()});

  @override
  void answer(
      String channel, Future<Message> Function(Message request) handler) {
    watch(channel).listen((event) async {
      Message response = await handler(event);
      publish(response);
    });
  }

  @override
  Future<void> connect(
    Uri uri, {
    String? username,
    String? password,
    String? identifier,
  }) async {
    await disconnect();

    List<String> allowedSchemes = ["mqtt", "mqtts", "ws", "wss"];
    if (!allowedSchemes.contains(uri.scheme)) {
      throw IllegalUriException(uri, "The scheme of the uri is not allowed!");
    }

    _client = factory.create(uri.toHost(), identifier ?? "")
      ..port = uri.port
      ..keepAlivePeriod = 20
      ..setProtocolV311()
      ..autoReconnect = true;

    _channel = uri.path;

    await _client?.connect(username, password);

    _client?.updates?.listen((event) {
      MqttPublishMessage rec = event[0].payload as MqttPublishMessage;
      final String topic = event[0].topic;
      List<int> messageAsBytes = rec.payload.message;
      String message = utf8.decode(messageAsBytes);

      for (final subscription in _subscribtions.entries) {
        String channel = Uri(
          path: "$_channel/${subscription.key}",
        ).toMqttChannel();

        RegExp regex = RegExp(
          "^${channel.replaceAll("/+", "/[^/]+").replaceAll("#", ".+")}\$",
        );

        if (!regex.hasMatch(topic)) {
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
      Uri(path: "$_channel/#").toMqttChannel(),
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
    Message request, {
    Duration timeout = const Duration(seconds: 30),
  }) async {
    Completer<String?> completer = Completer<String?>();
    StreamSubscription subscription = watch("${request.topic}/response").listen(
      (event) {
        completer.complete(event.payload);
      },
    );

    publish(request);
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

    _client?.publishMessage(
      Uri(path: "$_channel/${message.topic}").toMqttChannel(),
      MqttQos.exactlyOnce,
      builder.payload!,
    );
  }
}

extension _ToHost on Uri {
  String toHost() {
    String protocol = "";
    if (scheme == "wss") {
      protocol = "wss://";
    } else if (scheme == "ws") {
      protocol = "ws://";
    }
    return "$protocol$host";
  }

  String toMqttChannel() {
    List<String> channels = path.split("/");
    channels.removeWhere((element) => element.isEmpty);
    return channels.join("/");
  }
}
