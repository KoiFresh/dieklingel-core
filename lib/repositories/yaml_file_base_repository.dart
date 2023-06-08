import 'package:yaml/yaml.dart';

import 'package:path/path.dart' as path;
import 'file_base_repository.dart';

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
