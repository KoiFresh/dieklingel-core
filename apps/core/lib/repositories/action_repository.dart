import 'package:yaml/yaml.dart';

import '../extensions/yaml_map.dart';
import '../models/action.dart';
import 'yaml_file_base_repository.dart';

class ActionRepository extends YamlFileBaseRepository {
  Future<List<Action>> fetchAllActions() async {
    YamlMap config = await readYamlConfig();

    List<Action> actions = config
        .get<YamlList>("actions")
        .map((action) => Action.fromYaml(action))
        .toList();

    return actions;
  }

  Future<List<Action>> fetchActionsFor(String trigger) async {
    return (await fetchAllActions())
        .where((action) => action.trigger.hasMatch(trigger))
        .toList();
  }
}
