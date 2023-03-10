import 'package:yaml/yaml.dart';

enum SignType {
  none,
  lottie,
  html,
  image,
  native;
}

class SignOptions {
  final String identifier;
  final String file;
  final String? sound;
  int _type = SignType.none.index;

  SignType get type => SignType.values.firstWhere(
        (element) => element.index == _type,
        orElse: () => SignType.none,
      );

  SignOptions({
    required this.identifier,
    required this.file,
    required this.sound,
  }) {
    if (identifier.isEmpty) {
      throw "Cannot create Sign with an empty identifier";
    }
    if (file.isEmpty) {
      throw "Cannot create Sign $identifier with an empty file";
    }

    if (file.endsWith(".lottie") || file.endsWith(".json")) {
      _type = SignType.lottie.index;
    } else if (file.endsWith(".html")) {
      _type = SignType.html.index;
    } else if (file.endsWith(".png") ||
        file.endsWith(".jpeg") ||
        file.endsWith(".jpg")) {
      _type = SignType.image.index;
    } else if (file.endsWith(".dart")) {
      _type = SignType.native.index;
    } else {
      _type = SignType.none.index;
      throw "Cannot create Sign $identifier with file $file, extension is not supportet. Supportet extionses are: .lottie, .json, .html, .png, .jpeg, .jpg";
    }
  }

  factory SignOptions.fromYaml(YamlMap yaml) {
    return SignOptions(
      identifier: yaml["identifier"] ?? "",
      file: yaml["file"] ?? "",
      sound: yaml["sound"] ?? "",
    );
  }

  factory SignOptions.fromMap(Map<String, dynamic> map) {
    return SignOptions(
      identifier: map["identifier"] ?? "",
      file: map["file"] ?? "",
      sound: map["sound"] ?? "",
    );
  }

  Map<String, dynamic> toMap() {
    return {
      "identifier": identifier,
      "file": file,
      "sound": sound,
    };
  }

  @override
  String toString() {
    return "identifier: $identifier; file: $file; sound: $sound;";
  }
}
