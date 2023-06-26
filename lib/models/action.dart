import 'package:yaml/yaml.dart';

import '../extensions/yaml_map.dart';

class Action {
  final RegExp trigger;
  final String lane;

  Action({
    required this.trigger,
    required this.lane,
  });

  factory Action.fromYaml(YamlMap yaml) {
    RegExp trigger = RegExp(yaml.get<String>("trigger"));
    String lane = yaml.get<String>("run");

    return Action(
      trigger: trigger,
      lane: lane,
    );
  }

  Map<String, dynamic> toMap() {
    return {
      "trigger": trigger.pattern,
      "lane": lane,
    };
  }
}
