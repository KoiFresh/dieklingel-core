import 'dart:io';

import 'package:path/path.dart' as path;
import 'package:yaml/yaml.dart';

import '../extensions/yaml_map.dart';
import '../models/action.dart';

class ActionRepository {
  final String configFilePath =
      path.normalize("/usr/share/dieklingel/core.yaml");

  Future<List<Action>> fetchAllActions() async {
    File configFile = File(configFilePath);
    String rawConfig = await configFile.readAsString();
    YamlMap config = loadYaml(rawConfig);

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
