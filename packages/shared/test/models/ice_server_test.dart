import 'package:dieklingel_core_shared/dart_shared.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:yaml/yaml.dart';

void main() {
  group("IceServer", () {
    group("Constructor", () {
      test("with urls only", () {
        IceServer server = IceServer(urls: "stun:stun.l.google.com:19302");

        expect(server.urls, equals("stun:stun.l.google.com:19302"));
        expect(server.username, equals(""));
        expect(server.credential, equals(""));
      });

      test("with urls and username", () {
        IceServer server = IceServer(
          urls: "stun:stun.l.google.com:19302",
          username: "Username",
        );

        expect(server.urls, equals("stun:stun.l.google.com:19302"));
        expect(server.username, equals("Username"));
        expect(server.credential, equals(""));
      });

      test("with urls and credential", () {
        IceServer server = IceServer(
            urls: "stun:stun.l.google.com:19302",
            username: "Username",
            credential: "Credential");

        expect(server.urls, equals("stun:stun.l.google.com:19302"));
        expect(server.username, equals("Username"));
        expect(server.credential, equals("Credential"));
      });

      test(" with urls, username and credential", () {
        IceServer server = IceServer(
            urls: "stun:stun.l.google.com:19302",
            username: "Username",
            credential: "Credential");

        expect(server.urls, equals("stun:stun.l.google.com:19302"));
        expect(server.username, equals("Username"));
        expect(server.credential, equals("Credential"));
      });
    });

    group("fromMap(Map<String, dynamic> map)", () {
      test("with empty map", () {
        Map<String, dynamic> map = {};

        expect(() => IceServer.fromMap(map), throwsFormatException);
      });

      test("with number as urls", () {
        Map<String, dynamic> map = {
          "urls": 1,
        };

        expect(() => IceServer.fromMap(map), throwsFormatException);
      });

      test("with urls, username and credential", () {
        Map<String, dynamic> map = {
          "urls": "stun:stun.l.google.com:19302",
          "username": "Username",
          "credential": "Credential"
        };

        IceServer server = IceServer.fromMap(map);

        expect(server.urls, equals("stun:stun.l.google.com:19302"));
        expect(server.username, equals("Username"));
        expect(server.credential, equals("Credential"));
      });

      test("with number as username", () {
        Map<String, dynamic> map = {
          "urls": "stun:stun.l.google.com:19302",
          "username": 1,
          "credential": "Credential"
        };

        expect(() => IceServer.fromMap(map), throwsFormatException);
      });

      test("with number as credential", () {
        Map<String, dynamic> map = {
          "urls": "stun:stun.l.google.com:19302",
          "username": "Username",
          "credential": 1,
        };

        expect(() => IceServer.fromMap(map), throwsFormatException);
      });
    });

    group("fromYaml(YamlMap yaml)", () {
      test("with empty map", () {
        YamlMap map = YamlMap();

        expect(() => IceServer.fromYaml(map), throwsFormatException);
      });

      test("with number as urls", () {
        YamlMap map = YamlMap.wrap({
          "urls": 1,
        });

        expect(() => IceServer.fromYaml(map), throwsFormatException);
      });

      test("with urls, username and credential", () {
        YamlMap map = YamlMap.wrap({
          "urls": "stun:stun.l.google.com:19302",
          "username": "Username",
          "credential": "Credential"
        });

        IceServer server = IceServer.fromYaml(map);

        expect(server.urls, equals("stun:stun.l.google.com:19302"));
        expect(server.username, equals("Username"));
        expect(server.credential, equals("Credential"));
      });

      test("with number as username", () {
        YamlMap map = YamlMap.wrap({
          "urls": "stun:stun.l.google.com:19302",
          "username": 1,
          "credential": "Credential"
        });

        expect(() => IceServer.fromYaml(map), throwsFormatException);
      });

      test("with number as credential", () {
        YamlMap map = YamlMap.wrap({
          "urls": "stun:stun.l.google.com:19302",
          "username": "Username",
          "credential": 1,
        });

        expect(() => IceServer.fromYaml(map), throwsFormatException);
      });
    });

    group("toString()", () {
      test("with urls, username and credential", () {
        IceServer server = IceServer(
          urls: "stun:stun.l.google.com:19302",
          username: "Username",
          credential: "Credential",
        );

        String result = server.toString();
        String expected =
            "IceServer urls: stun:stun.l.google.com:19302, username: Username, credential: Credential";

        expect(result, equals(expected));
      });
    });

    group("toMap()", () {
      test("with urls", () {
        IceServer server = IceServer(urls: "stun:stun.l.google.com:19302");
        Map<String, dynamic> expected = {
          "urls": "stun:stun.l.google.com:19302",
          "username": "",
          "credential": "",
        };

        Map<String, dynamic> result = server.toMap();

        expect(result, equals(expected));
      });

      test("with urls, username and credential", () {
        IceServer server = IceServer(
          urls: "stun:stun.l.google.com:19302",
          username: "Username",
          credential: "Credential",
        );
        Map<String, dynamic> expected = {
          "urls": "stun:stun.l.google.com:19302",
          "username": "Username",
          "credential": "Credential",
        };

        Map<String, dynamic> result = server.toMap();

        expect(result, equals(expected));
      });
    });

    group("operator ==(Object other)", () {
      test("with self", () {
        IceServer server = IceServer(urls: "stun:stun.l.google.com:19302");

        bool result = server == server;

        expect(result, equals(true));
      });

      test("with other instance, same values", () {
        IceServer server = IceServer(urls: "stun:stun.l.google.com:19302");
        IceServer other = IceServer(urls: "stun:stun.l.google.com:19302");

        bool result = server == other;

        expect(result, equals(true));
      });

      test("with other instance, different values", () {
        IceServer server = IceServer(urls: "stun:stun.l.google.com:19302");
        IceServer other = IceServer(urls: "stun");

        bool result = server == other;

        expect(result, equals(false));
      });

      test("with conversion to and from map", () {
        IceServer server = IceServer(urls: "stun:stun.l.google.com:19302");
        IceServer other = IceServer.fromMap(server.toMap());

        bool result = server == other;

        expect(result, equals(true));
      });

      test("with other instance and empty username and credential", () {
        IceServer server = IceServer(urls: "stun:stun.l.google.com:19302");
        IceServer other = IceServer(
          urls: "stun:stun.l.google.com:19302",
          username: "",
          credential: "",
        );

        bool result = server == other;

        expect(result, equals(true));
      });
    });
  });
}
