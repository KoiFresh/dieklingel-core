import 'package:flutter/foundation.dart';
import 'package:mqtt_client/mqtt_browser_client.dart';
import 'package:mqtt_client/mqtt_client.dart';

MqttClient factory(
  String hostname,
  String identifier, {
  int maxConnectionAttempts = 3,
}) {
  MqttClient client = MqttBrowserClient(
    hostname,
    identifier,
    maxConnectionAttempts: maxConnectionAttempts,
  );

  if (kIsWeb) {
    client.websocketProtocols = MqttClientConstants.protocolsSingleDefault;
  }

  return client;
}
