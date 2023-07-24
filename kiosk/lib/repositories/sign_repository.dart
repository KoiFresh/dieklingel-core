import 'dart:io';

import 'package:yaml/yaml.dart';

import '../extensions/yaml_map.dart';
import '../models/sign.dart';
import 'yaml_file_base_repository.dart';

class SignRepository extends YamlFileBaseRepository {
  Future<List<Sign>> fetchAllSigns() async {
    YamlMap config = await readYamlConfig();

    List<Sign> signs =
        config.get<YamlMap>("gui").get<YamlList>("signs").cast<YamlMap>().map(
      (YamlMap e) {
        String identifier = e.get<String>("name");
        File audio = File(e.get<String>("audio"));
        File interface = File(e.get<String>("interface"));

        return Sign(identifier: identifier, audio: audio, interface: interface);
      },
    ).toList();

    return signs;
  }
}
