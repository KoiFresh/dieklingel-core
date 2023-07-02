import 'dart:io';

import 'package:yaml/yaml.dart';

import '../blueprint/blueprint.dart';
import '../extensions/yaml_map.dart';

class Sign {
  final String identifier;
  final File audio;
  final File interface;

  Sign({
    required this.identifier,
    required this.audio,
    required this.interface,
  });

  factory Sign.fromYaml(YamlMap yaml) {
    matchMap(
      yaml.cast(),
      {
        "identifier": StringF,
        "audio": FileF,
        "interface": FileF,
      },
    );

    String identifier = yaml.get<String>("identifier");
    File audio = File(yaml.get<String>("audio"));
    File interface = File(yaml.get<String>("interface"));

    return Sign(
      identifier: identifier,
      audio: audio,
      interface: interface,
    );
  }
}
