import 'dart:async';

import 'package:shelf/shelf.dart';

import 'models/legacy_mqtt_response.dart';
import 'models/message.dart';
import 'models/mqtt_uri.dart';

abstract class IClient {
  Future<void> serve(Handler handler, Uri host);

  /// Creates a mqtt request with the given [request] parameters.
  //Future<Response> request(Request request);

  /// Registers a listener wich handles all requests for the given channel.
  /* void answer(
    String channel,
    Future<Response> Function(Request request) handler,
  ); */

  void legacyAnswer(
    String channel,
    Future<LegacyMqttResponse> Function(String request) handler,
  );

  Future<void> connect(
    MqttUri uri, {
    String? username,
    String? password,
    String? identifier,
  });

  Future<void> disconnect();

  Stream<Message> watch(String channel);

  void publish(Message message);
}
