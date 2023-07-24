import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';

MqttClient factory(
  String hostname,
  String identifier, {
  int maxConnectionAttempts = 3,
}) {
  MqttClient client = MqttServerClient(
    hostname,
    identifier,
    maxConnectionAttempts: maxConnectionAttempts,
  );

  return client;
}
