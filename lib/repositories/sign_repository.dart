import 'dart:io';

import 'package:path/path.dart' as path;
import 'package:yaml/yaml.dart';

import '../extensions/yaml_map.dart';
import '../models/sign.dart';

class SignRepository {
  final String configFilePath =
      path.normalize("/usr/share/dieklingel/core.yaml");

  Future<List<Sign>> fetchAllSigns() async {
    File configFile = File(configFilePath);
    String rawConfig = await configFile.readAsString();
    YamlMap config = loadYaml(rawConfig);

    List<Sign> signs = config
        .get<YamlMap>("gui")
        .get<YamlList>("signs")
        .map((action) => Sign.fromYaml(action))
        .toList();

    return signs;
  }
}
