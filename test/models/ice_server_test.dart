import 'package:core/blueprint/blueprint.dart';
import 'package:core/models/ice_server.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:yaml/yaml.dart';

void main() {
  group("Test IceServer", () {
    group("Test IceServer constructor", () {
      test("Test with urls only", () {
        final IceServer server = IceServer(urls: "stun:stun.l.google.com");

        expect(server.urls, equals("stun:stun.l.google.com"));
        expect(server.username, isEmpty);
        expect(server.credential, isEmpty);
      });

      test("Test with empty values", () {
        final IceServer server = IceServer(
          urls: "",
          username: "",
          credential: "",
        );

        expect(server.urls, isEmpty);
        expect(server.username, isEmpty);
        expect(server.credential, isEmpty);
      });

      test("Test with all values", () {
        final IceServer server = IceServer(
          urls: "stun:stun.l.google.com:19302",
          username: "username",
          credential: "password",
        );

        expect(server.urls, equals("stun:stun.l.google.com:19302"));
        expect(server.username, equals("username"));
        expect(server.credential, equals("password"));
      });
    });

    group("Test IceServer fromMap", () {
      test("Test with empty map", () {
        expect(
          () => IceServer.fromMap({}),
          throwsA(isA<BluePrintException>()),
        );
      });

      test("Test with int as urls", () {
        expect(
          () => IceServer.fromMap({
            "urls": 1,
          }),
          throwsA(isA<BluePrintException>()),
        );
      });

      test("Test with all values", () {
        final IceServer server = IceServer.fromMap(
          {
            "urls": "stun:stun.l.google.com:19302",
            "username": "username",
            "credential": "password"
          },
        );

        expect(server.urls, equals("stun:stun.l.google.com:19302"));
        expect(server.username, equals("username"));
        expect(server.credential, equals("password"));
      });

      test("Test with urls only", () {
        final IceServer server = IceServer.fromMap(
          {
            "urls": "stun:stun.l.google.com:19302",
          },
        );

        expect(server.urls, equals("stun:stun.l.google.com:19302"));
        expect(server.username, isEmpty);
        expect(server.credential, isEmpty);
      });
    });

    group("Test IceServer fromYaml", () {
      test("Test with empty map", () {
        expect(
          () => IceServer.fromYaml(YamlMap.wrap({})),
          throwsA(isA<BluePrintException>()),
        );
      });

      test("Test with int as urls", () {
        expect(
          () => IceServer.fromYaml(
            YamlMap.wrap(
              {
                "urls": 1,
              },
            ),
          ),
          throwsA(isA<BluePrintException>()),
        );
      });

      test("Test with all values", () {
        final IceServer server = IceServer.fromYaml(
          YamlMap.wrap(
            {
              "urls": "stun:stun.l.google.com:19302",
              "username": "username",
              "credential": "password"
            },
          ),
        );

        expect(server.urls, equals("stun:stun.l.google.com:19302"));
        expect(server.username, equals("username"));
        expect(server.credential, equals("password"));
      });

      test("Test with urls only", () {
        final IceServer server = IceServer.fromYaml(
          YamlMap.wrap(
            {
              "urls": "stun:stun.l.google.com:19302",
            },
          ),
        );

        expect(server.urls, equals("stun:stun.l.google.com:19302"));
        expect(server.username, isEmpty);
        expect(server.credential, isEmpty);
      });
    });

    group("Test IceServer toString", () {
      test("Test with urls only", () {
        final IceServer server = IceServer(urls: "stun.l.google.com");

        expect(
          server.toString(),
          equals(
            "IceServer: urls->'stun.l.google.com', username->'', credential->''",
          ),
        );
      });

      test("Test with all values", () {
        final IceServer server = IceServer(
          urls: "stun.l.google.com",
          username: "username",
          credential: "password",
        );

        expect(
          server.toString(),
          equals(
            "IceServer: urls->'stun.l.google.com', username->'username', credential->'password'",
          ),
        );
      });
    });

    group("Test IceServer toMap", () {
      test("Test with empty values", () {
        final IceServer server = IceServer(urls: "");

        expect(
          server.toMap(),
          equals(
            {
              "urls": "",
              "username": "",
              "credential": "",
            },
          ),
        );
      });

      test("Test with all values", () {
        final IceServer server = IceServer(
          urls: "stun:stun.l.google.com:19302",
          username: "username",
          credential: "password",
        );

        expect(
          server.toMap(),
          equals(
            {
              "urls": "stun:stun.l.google.com:19302",
              "username": "username",
              "credential": "password",
            },
          ),
        );
      });
    });

    group("Test IceServer ==", () {
      test("Test with all values", () {
        final IceServer server1 = IceServer(
          urls: "stun:stun.l.google.com:19302",
          username: "username",
          credential: "password",
        );

        final IceServer server2 = IceServer(
          urls: "stun:stun.l.google.com:19302",
          username: "username",
          credential: "password",
        );

        expect(server1, equals(server2));
      });

      test("Test with all urls only", () {
        final IceServer server1 = IceServer(
          urls: "stun:stun.l.google.com:19302",
        );

        final IceServer server2 = IceServer(
          urls: "stun:stun.l.google.com:19302",
        );

        expect(server1, equals(server2));
      });

      test("Test with toMap and fromMap", () {
        final IceServer server1 = IceServer(
          urls: "stun:stun.l.google.com:19302",
        );

        final IceServer server2 = IceServer.fromMap(server1.toMap());

        expect(server1, equals(server2));
      });

      test("Test with ==", () {
        final IceServer server1 = IceServer(
          urls: "stun:stun.l.google.com:19302",
        );

        expect(server1 == server1, equals(true));
      });

      test("Test with == and server1 and server2", () {
        final IceServer server1 = IceServer(
          urls: "stun:stun.l.google.com:19302",
        );

        final IceServer server2 = IceServer(
          urls: "stun:stun.l.google.com:19302",
        );

        expect(server1 == server2, equals(true));
      });

      test("Test with != and server1 and server2", () {
        final IceServer server1 = IceServer(
          urls: "stun:stun.l.google.com:19302",
        );

        final IceServer server2 = IceServer(
          urls: "stun:stun.l.notgoogle.com:19302",
        );

        expect(server1 != server2, equals(true));
      });
    });
  });
}
