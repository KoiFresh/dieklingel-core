import 'package:mqtt/models/request.dart';

import 'models/message.dart';
import 'models/mqtt_uri.dart';
import 'models/response.dart';
import 'models/connection_state.dart';

abstract class IClient {
  ConnectionState get state;

  /// Creates a mqtt request with the given [request] parameters.
  Future<Response> request(Request request);

  /// Registers a listener wich handles all requests for the given channel.
  void answer(
    String channel,
    Future<Response> Function(Request request) handler,
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
