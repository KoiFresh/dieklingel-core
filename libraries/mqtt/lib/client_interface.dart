import 'models/message.dart';
import 'models/mqtt_uri.dart';
import 'models/response.dart';
import 'models/connection_state.dart';

abstract class ClientInterface {
  ConnectionState get state;

  /// Creates a mqtt request with the given [request] parameters.
  Future<Response> request(Message request);

  /// Registers a listener wich handles all requests for the given channel.
  void answer(
    String channel,
    Future<Message> Function(Message request) handler,
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
