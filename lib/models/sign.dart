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
        "audio": FileF.exist(),
        "interface": FileF.exist(),
      },
      throwable: true,
    );

    String identifier = yaml.get<String>("identifier");
    File audio = File(yaml.get<String>("audio"));
    File interface = File(yaml.get<String>("interface"));

    if (identifier.isEmpty) {
      throw BluePrintException(
        key: "identifier",
        msg: "Cannot create a Sign with an empty identifier",
      );
    }

    return Sign(
      identifier: identifier,
      audio: audio,
      interface: interface,
    );
  }
}
