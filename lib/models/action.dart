import 'package:yaml/yaml.dart';

import '../blueprint/blueprint.dart';
import '../extensions/yaml_map.dart';

class Action {
  final RegExp trigger;
  final String lane;

  Action({
    required this.trigger,
    required this.lane,
  });

  factory Action.fromYaml(YamlMap yaml) {
    matchMap(
      yaml.cast(),
      {
        "trigger": StringF,
        "lane": StringF,
      },
      throwable: true,
    );

    RegExp trigger = RegExp(yaml.get<String>("trigger"));
    String lane = yaml.get<String>("lane");

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
