import 'package:yaml/yaml.dart';

import 'trigger.dart';

class Event {
  final Trigger trigger;
  final String command;

  Event({
    required this.trigger,
    required this.command,
  });

  factory Event.fromYaml(YamlMap yaml) {
    Trigger trigger;
    String run;

    if (yaml["trigger"] is! String) {
      throw FormatException(
        "The Event is missing the trigger propertie.",
        yaml,
      );
    }

    trigger = Trigger.fromString(yaml["trigger"]);

    if (yaml["run"] is! String) {
      throw FormatException(
        "The Event is missing the run propertie.",
        yaml,
      );
    }

    run = yaml["run"];

    return Event(trigger: trigger, command: run);
  }
}
