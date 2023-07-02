import 'package:core/blueprint/blueprint.dart';
import 'package:core/models/action.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:yaml/yaml.dart';

void main() {
  group("Test Action constructor", () {
    test("Test with empty trigger and lane", () {
      final Action action = Action(
        trigger: RegExp(""),
        lane: "",
      );

      expect(action.trigger, equals(RegExp("")));
      expect(action.lane, equals(""));
    });
  });

  group("Test Action fromYaml", () {
    test("Test with empty values", () {
      final Action action = Action.fromYaml(YamlMap.wrap(
        {
          "trigger": "",
          "lane": "",
        },
      ));

      expect(action.trigger, equals(RegExp("")));
      expect(action.lane, equals(""));
    });

    test("Test with empty map", () {
      expect(
        () => Action.fromYaml(YamlMap.wrap({})),
        throwsA(isA<BluePrintException>()),
      );
    });

    test("Test without run", () {
      expect(
        () => Action.fromYaml(YamlMap.wrap({
          "trigger": "",
        })),
        throwsA(isA<BluePrintException>()),
      );
    });

    test("Test with int istead of strings", () {
      expect(
        () => Action.fromYaml(YamlMap.wrap({
          "trigger": 1,
          "run": 2,
        })),
        throwsA(isA<BluePrintException>()),
      );
    });
  });

  group("Test Action toMap", () {
    test("Test with empty values", () {
      final Action action = Action(trigger: RegExp(""), lane: "");
      final Map<String, dynamic> map = action.toMap();

      expect(
        map,
        equals(
          {
            "trigger": "",
            "lane": "",
          },
        ),
      );
    });

    test("Test with values", () {
      final Action action = Action(trigger: RegExp("unlock"), lane: "ls -l");
      final Map<String, dynamic> map = action.toMap();

      expect(
        map,
        equals(
          {
            "trigger": "unlock",
            "lane": "ls -l",
          },
        ),
      );
    });
  });
}
