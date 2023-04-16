import 'package:flutter_test/flutter_test.dart';
import 'package:mockito/annotations.dart';
import 'package:mockito/mockito.dart';
import 'package:mqtt/client.dart';
import 'package:mqtt/exceptions/illegal_uri_exception.dart';
import 'package:mqtt/factories/mqtt_client_factory.dart';
import 'package:mqtt/models/connection_state.dart';
import 'package:mqtt/models/message.dart';
import 'package:mqtt_client/mqtt_client.dart';

import 'client_test.mocks.dart';

@GenerateMocks([MqttClient, MqttClientFactory])
void main() {
  group("Client", () {
    final factory = MockMqttClientFactory();

    setUp(() {
      when(factory.create("server.dieklingel.com", "ident")).thenAnswer(
        (_) {
          final client = MockMqttClient();

          when(client.connect()).thenAnswer(
            (_) async => MqttClientConnectionStatus()
              ..state = MqttConnectionState.connected,
          );

          when(client.connectionStatus).thenAnswer(
            (_) => MqttClientConnectionStatus()
              ..state = MqttConnectionState.connected,
          );

          when(client.updates).thenAnswer((_) => null);

          when(client.subscribe(any, any)).thenAnswer(
            (_) => Subscription()..qos = MqttQos.exactlyOnce,
          );

          return client;
        },
      );
    });

    test("initial state is disconnected", () {
      final client = Client(factory: factory);

      expect(client.state, equals(ConnectionState.disconnected));
    });

    test("connect with http uri", () {
      final client = Client(factory: factory);
      final uri = Uri.parse("http://server.dieklingel.com");

      expect(
        () async => await client.connect(uri),
        throwsA(isA<IllegalUriException>()),
      );
    });

    test("client connects", () async {
      final client = Client(factory: factory);
      final uri = Uri.parse("mqtt://server.dieklingel.com:1883/");

      await client.connect(uri, identifier: "ident");

      expect(client.state, equals(ConnectionState.connected));
    });

    test("client connects and disconnects", () async {
      final client = Client(factory: factory);
      final uri = Uri.parse("mqtt://server.dieklingel.com:1883/");

      await client.connect(uri, identifier: "ident");

      expect(client.state, equals(ConnectionState.connected));

      await client.disconnect();

      expect(client.state, equals(ConnectionState.disconnected));
    });

    test("client disconnects without beeing connected", () async {
      final client = Client(factory: factory);

      await client.disconnect();

      expect(client.state, equals(ConnectionState.disconnected));
    });
  });

  group("Connnected", () {
    final factory = MockMqttClientFactory();
    late final MockMqttClient mqttclient;
    late final Client client;

    setUp(() async {
      mqttclient = MockMqttClient();

      when(mqttclient.connect()).thenAnswer(
        (_) async =>
            MqttClientConnectionStatus()..state = MqttConnectionState.connected,
      );

      when(mqttclient.connectionStatus).thenAnswer(
        (_) =>
            MqttClientConnectionStatus()..state = MqttConnectionState.connected,
      );

      when(mqttclient.subscribe(any, any)).thenAnswer(
        (_) => Subscription()..qos = MqttQos.exactlyOnce,
      );

      when(mqttclient.updates).thenAnswer((_) => null);

      when(mqttclient.publishMessage(any, any, any)).thenAnswer((_) => 1);

      when(factory.create("server.dieklingel.com", "ident")).thenAnswer(
        (_) => mqttclient,
      );

      client = Client(factory: factory);
      await client.connect(
        Uri.parse("mqtt://server.dieklingel.com:1883/test"),
        identifier: "ident",
      );
    });

    test("publish", () {
      client.publish(Message("/test", "Hallo Welt"));

      verify(mqttclient.publishMessage(any, any, any)).called(1);

      client.publish(Message("/test", "Hallo Welt"));
      client.publish(Message("/test", "Hallo Welt"));

      verify(mqttclient.publishMessage(any, any, any)).called(2);

      client.publish(Message("/test", ""));

      verify(mqttclient.publishMessage(any, any, any)).called(1);
    });
  });
}
