import 'package:dieklingel_core_shared/mqtt/mqtt_response.dart';
import 'package:flutter_test/flutter_test.dart';

void main() {
  group("MqttResponse", () {
    test(".()", () {
      const MqttResponse response = MqttResponse(status: 200, message: "Fine");

      expect(response.status, equals(200));
      expect(response.message, equals("Fine"));
      expect(response.body, equals(null));
    });

    group(".fromMap()", () {
      test("without all parameters", () {
        Map<String, dynamic> map = {
          "status": 200,
          "message": "Ok",
          "body": 100,
        };

        MqttResponse response = MqttResponse.fromMap(map);

        expect(response.status, equals(200));
        expect(response.message, equals("Ok"));
        expect(response.body, equals(100));
      });

      test("without status", () {
        Map<String, dynamic> map = {
          "message": "Ok",
          "body": {},
        };

        expect(() => MqttResponse.fromMap(map), throwsException);
      });

      test("without message", () {
        Map<String, dynamic> map = {
          "status": 200,
          "body": {},
        };

        expect(() => MqttResponse.fromMap(map), throwsException);
      });

      test("without body", () {
        Map<String, dynamic> map = {
          "status": 200,
          "message": "Ok",
        };

        MqttResponse response = MqttResponse.fromMap(map);
        expect(response.body, equals(null));
      });
    });

    test(".toMap()", () {
      const MqttResponse response = MqttResponse(
        status: 200,
        message: "Everything Good",
        body: {},
      );

      Map<String, dynamic> map = response.toMap();

      expect(map["status"], equals(200));
      expect(map["message"], equals("Everything Good"));
      expect(map["body"], equals({}));
    });

    test(".toString()", () {
      const MqttResponse response = MqttResponse(status: 200, message: "Fine");

      expect(
        response.toString(),
        equals("MqttResponse: status: 200, message: Fine, body: null"),
      );
    });
  });
}
