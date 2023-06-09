import 'dart:io';

import 'package:yaml/yaml.dart';

import '../extensions/yaml_map.dart';
import '../models/sign.dart';
import 'yaml_file_base_repository.dart';

class SignRepository extends YamlFileBaseRepository {
  Future<List<Sign>> fetchAllSigns() async {
    YamlMap config = await readYamlConfig();

    List<Sign> signs = config
        .get<YamlMap>("gui")
        .get<YamlMap>("signs")
        .map<String, YamlMap>((key, value) => MapEntry(key, value))
        .entries
        .map(
      (e) {
        String identifier = e.key;
        File audio = File(e.value.get<String>("audio"));
        File interface = File(e.value.get<String>("interface"));

        return Sign(identifier: identifier, audio: audio, interface: interface);
      },
    ).toList();

    return signs;
  }
}
