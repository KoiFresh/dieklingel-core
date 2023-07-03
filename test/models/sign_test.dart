import 'dart:io';

import 'package:core/blueprint/blueprint.dart';
import 'package:core/models/sign.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:yaml/yaml.dart';

import '../mocks/fake_file.dart';

void main() {
  group("Test Sign fromYaml", () {
    test("Test with empty map", () {
      expect(
        () => Sign.fromYaml(YamlMap.wrap({})),
        throwsA(isA<BluePrintException>()),
      );
    });

    test("Test with existing file", () {
      IOOverrides.runZoned(
        () {
          final Sign sign = Sign.fromYaml(YamlMap.wrap({
            "identifier": "ident",
            "audio": "audio.wav",
            "interface": "interface.rfwtxt"
          }));

          expect(sign.identifier, equals("ident"));
          expect(sign.audio, equals(File("audio.wav")));
          expect(sign.interface, equals(File("interface.rfwtxt")));
        },
        createFile: (p0) => FakeFile(p0, fileExists: true),
      );
    });

    test("Test with non existing file", () {
      IOOverrides.runZoned(
        () {
          expect(
            () => Sign.fromYaml(
              YamlMap.wrap({
                "identifier": "ident",
                "audio": "non-existing-audio.wav",
                "interface": "non-existing-interface.rfwtxt"
              }),
            ),
            throwsA(isA<BluePrintException>()),
          );
        },
        createFile: (p0) => FakeFile(p0, fileExists: false),
      );
    });

    test("Test with empty identifier", () {
      IOOverrides.runZoned(
        () {
          expect(
            () => Sign.fromYaml(
              YamlMap.wrap({
                "identifier": "",
                "audio": "audio.wav",
                "interface": "interface.rfwtxt"
              }),
            ),
            throwsA(isA<BluePrintException>()),
          );
        },
        createFile: (p0) => FakeFile(p0, fileExists: true),
      );
    });
  });
}
