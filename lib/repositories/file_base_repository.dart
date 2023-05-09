import 'dart:io';

import 'package:path/path.dart' as path;
import 'package:yaml/yaml.dart';

abstract class FileBaseRepository {
  String get configFilePath;

  Future<String> readConfigFile() async {
    File configFile = File(configFilePath);
    String rawConfig = await configFile.readAsString();

    return rawConfig;
  }
}

abstract class YamlFileBaseRepository extends FileBaseRepository {
  String get configFilePath =>
      path.normalize("/usr/share/dieklingel/core.yaml");

  Future<YamlMap> readYamlConfig() async {
    String rawConfig = await readConfigFile();
    YamlMap config = loadYaml(rawConfig);

    return config;
  }
}
