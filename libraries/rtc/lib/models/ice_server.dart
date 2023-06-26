import 'package:yaml/yaml.dart';

class IceServer {
  String urls;
  String username;
  String credential;

  IceServer({
    required this.urls,
    this.username = "",
    this.credential = "",
  });

  factory IceServer.fromMap(Map<String, dynamic> map) {
    if (map["urls"] is! String) {
      throw FormatException(
        "In order to create a IceServer from a map, the map has to have a key 'urls' of type String.",
        map,
      );
    }

    if (map["username"] != null && map["username"] is! String) {
      throw FormatException(
        "In order to create a IceServer from a map, the key 'username' has to be of type String, null or not existing.",
        map,
      );
    }

    if (map["credential"] != null && map["credential"] is! String) {
      throw FormatException(
        "In order to create a IceServer from a map, the key 'credential' has to be of type String, null or not existing.",
        map,
      );
    }

    return IceServer(
      urls: map["urls"],
      username: map["username"] ?? "",
      credential: map["credential"] ?? "",
    );
  }

  factory IceServer.fromYaml(YamlMap yaml) {
    if (yaml["urls"] is! String) {
      throw FormatException(
        "In order to create a IceServer from yaml, the yaml has to have a key 'urls' of type String.",
        yaml,
      );
    }

    if (yaml["username"] != null && yaml["username"] is! String) {
      throw FormatException(
        "In order to create a IceServer from yaml, the key 'username' has to be of type String, null or not existing.",
        yaml,
      );
    }

    if (yaml["credential"] != null && yaml["credential"] is! String) {
      throw FormatException(
        "In order to create a IceServer from yaml, the key 'credential' has to be of type String, null or not existing.",
        yaml,
      );
    }

    return IceServer(
      urls: yaml["urls"],
      username: yaml["username"] ?? "",
      credential: yaml["credential"] ?? "",
    );
  }

  @override
  String toString() {
    return "IceServer urls: $urls, username: $username, credential: $credential";
  }

  Map<String, dynamic> toMap() {
    return {
      "urls": urls,
      "username": username,
      "credential": credential,
    };
  }

  @override
  bool operator ==(Object other) {
    if (other is! IceServer) {
      return false;
    }
    return urls == other.urls &&
        username == other.username &&
        credential == other.credential;
  }

  @override
  int get hashCode => Object.hash(urls, username, credential);
}
