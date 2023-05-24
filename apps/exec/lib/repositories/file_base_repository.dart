import 'dart:io';

import 'package:path/path.dart' as path;
import 'package:yaml/yaml.dart';

abstract class FileBaseRepository {
  String get homePath =>
      Platform.environment["SNAP_REAL_HOME"] ??
      Platform.environment["HOME"] ??
      "";

  String get configFilePath;

  Future<String> readConfigFile() async {
    File configFile = File(configFilePath);
    String rawConfig = await configFile.readAsString();

    return rawConfig;
  }
}

abstract class YamlFileBaseRepository extends FileBaseRepository {
  @override
  String get configFilePath => path.join(
        homePath,
        "dieklingel",
        "core.yaml",
      );

  Future<YamlMap> readYamlConfig() async {
    String rawConfig = await readConfigFile();
    YamlMap config = loadYaml(rawConfig);

    return config;
  }
}
