import 'package:flutter_test/flutter_test.dart';
import 'package:mqtt/models/mqtt_uri.dart';

void main() {
  group("MqttUri", () {
    group("Constructor", () {
      test("with default parameters", () {
        MqttUri uri = MqttUri(host: "server.dieklingel.com", port: 1883);

        expect(uri.host, equals("server.dieklingel.com"));
        expect(uri.port, equals(1883));
        expect(uri.channel, equals(""));
        expect(uri.section, equals(""));
        expect(uri.ssl, equals(true));
        expect(uri.websocket, equals(false));
      });

      test("with default parameters and channel and section", () {
        MqttUri uri = MqttUri(
          host: "server.dieklingel.com",
          port: 1883,
          channel: "com.dieklingel/channel/",
          section: "default",
        );

        expect(uri.host, equals("server.dieklingel.com"));
        expect(uri.port, equals(1883));
        expect(uri.channel, equals("com.dieklingel/channel"));
        expect(uri.section, equals("default"));
        expect(uri.ssl, equals(true));
        expect(uri.websocket, equals(false));
      });
    });

    group("fromMap", () {
      test("with empty map", () {
        Map<String, dynamic> map = {};

        expect(() => MqttUri.fromMap(map), throwsFormatException);
      });

      test("with host", () {
        Map<String, dynamic> map = {
          "host": "server.diekligel.com",
        };

        expect(() => MqttUri.fromMap(map), throwsFormatException);
      });

      test("with host and port", () {
        Map<String, dynamic> map = {
          "host": "server.dieklingel.com",
          "port": 1883,
        };

        MqttUri uri = MqttUri.fromMap(map);

        expect(uri.host, equals("server.dieklingel.com"));
        expect(uri.port, equals(1883));
        expect(uri.channel, equals(""));
        expect(uri.section, equals(""));
        expect(uri.ssl, equals(true));
        expect(uri.websocket, equals(false));
      });

      test("with number as host", () {
        Map<String, dynamic> map = {
          "host": 1883,
          "port": 1883,
        };

        expect(() => MqttUri.fromMap(map), throwsFormatException);
      });

      test("with string as port", () {
        Map<String, dynamic> map = {
          "host": "server.dieklingel.com",
          "port": "1883",
        };

        expect(() => MqttUri.fromMap(map), throwsFormatException);
      });

      test("with number as channel", () {
        Map<String, dynamic> map = {
          "host": "server.dieklingel.com",
          "port": 1883,
          "channel": 1,
        };

        expect(() => MqttUri.fromMap(map), throwsFormatException);
      });

      test("with number as section", () {
        Map<String, dynamic> map = {
          "host": "server.dieklingel.com",
          "port": 1883,
          "section": 1,
        };

        expect(() => MqttUri.fromMap(map), throwsFormatException);
      });

      test("with number as ssl", () {
        Map<String, dynamic> map = {
          "host": "server.dieklingel.com",
          "port": 1883,
          "ssl": 1,
        };

        expect(() => MqttUri.fromMap(map), throwsFormatException);
      });

      test("with number as websocket", () {
        Map<String, dynamic> map = {
          "host": "server.dieklingel.com",
          "port": 1883,
          "websocket": 1,
        };

        expect(() => MqttUri.fromMap(map), throwsFormatException);
      });

      test("with all values", () {
        Map<String, dynamic> map = {
          "host": "server.dieklingel.com",
          "port": 1883,
          "channel": "com.dieklingel/channel/",
          "section": "default",
          "ssl": false,
          "websocket": true,
        };

        MqttUri uri = MqttUri.fromMap(map);

        expect(uri.host, equals("server.dieklingel.com"));
        expect(uri.port, equals(1883));
        expect(uri.channel, equals("com.dieklingel/channel"));
        expect(uri.section, equals("default"));
        expect(uri.ssl, equals(false));
        expect(uri.websocket, equals(true));
      });
    });

    group("fromUri", () {
      test("with mqtt", () {
        Uri uri = Uri.parse(
          "mqtt://server.dieklingel.com:1883/com.dieklingel/channel/#section",
        );

        MqttUri mqttUri = MqttUri.fromUri(uri);

        expect(mqttUri.host, equals("server.dieklingel.com"));
        expect(mqttUri.port, equals(1883));
        expect(mqttUri.channel, equals("com.dieklingel/channel"));
        expect(mqttUri.section, equals("section"));
        expect(mqttUri.ssl, equals(false));
        expect(mqttUri.websocket, equals(false));
      });

      test("with mqtts", () {
        Uri uri = Uri.parse(
          "mqtts://server.dieklingel.com:1883/com.dieklingel/channel/#section",
        );

        MqttUri mqttUri = MqttUri.fromUri(uri);

        expect(mqttUri.host, equals("server.dieklingel.com"));
        expect(mqttUri.port, equals(1883));
        expect(mqttUri.channel, equals("com.dieklingel/channel"));
        expect(mqttUri.section, equals("section"));
        expect(mqttUri.ssl, equals(true));
        expect(mqttUri.websocket, equals(false));
      });

      test("with ws", () {
        Uri uri = Uri.parse(
          "ws://server.dieklingel.com:9001/com.dieklingel/channel/#section",
        );

        MqttUri mqttUri = MqttUri.fromUri(uri);

        expect(mqttUri.host, equals("server.dieklingel.com"));
        expect(mqttUri.port, equals(9001));
        expect(mqttUri.channel, equals("com.dieklingel/channel"));
        expect(mqttUri.section, equals("section"));
        expect(mqttUri.ssl, equals(false));
        expect(mqttUri.websocket, equals(true));
      });

      test("with wss", () {
        Uri uri = Uri.parse(
          "wss://server.dieklingel.com:9001/com.dieklingel/channel/#section",
        );

        MqttUri mqttUri = MqttUri.fromUri(uri);

        expect(mqttUri.host, equals("server.dieklingel.com"));
        expect(mqttUri.port, equals(9001));
        expect(mqttUri.channel, equals("com.dieklingel/channel"));
        expect(mqttUri.section, equals("section"));
        expect(mqttUri.ssl, equals(true));
        expect(mqttUri.websocket, equals(true));
      });

      test("without root channel and empty section", () {
        Uri uri = Uri.parse(
          "mqtt://server.dieklingel.com:1883/#",
        );

        MqttUri mqttUri = MqttUri.fromUri(uri);

        expect(mqttUri.host, equals("server.dieklingel.com"));
        expect(mqttUri.port, equals(1883));
        expect(mqttUri.channel, equals(""));
        expect(mqttUri.section, equals(""));
        expect(mqttUri.ssl, equals(false));
        expect(mqttUri.websocket, equals(false));
      });
    });

    test("without channel and  section", () {
      Uri uri = Uri.parse(
        "mqtt://server.dieklingel.com:1883",
      );

      MqttUri mqttUri = MqttUri.fromUri(uri);

      expect(mqttUri.host, equals("server.dieklingel.com"));
      expect(mqttUri.port, equals(1883));
      expect(mqttUri.channel, equals(""));
      expect(mqttUri.section, equals(""));
      expect(mqttUri.ssl, equals(false));
      expect(mqttUri.websocket, equals(false));
    });
  });
}
