import 'package:yaml/yaml.dart';

import '../extensions/yaml_map.dart';
import '../models/sign.dart';
import 'file_base_repository.dart';

class SignRepository extends YamlFileBaseRepository {
  Future<List<Sign>> fetchAllSigns() async {
    YamlMap config = await readYamlConfig();

    List<Sign> signs = config
        .get<YamlMap>("gui")
        .get<YamlList>("signs")
        .map((action) => Sign.fromYaml(action))
        .toList();

    return signs;
  }
}
