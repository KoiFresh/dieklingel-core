import 'package:yaml/yaml.dart';

import '../blueprint/blueprint.dart';

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
    matchMap(
      map,
      {
        "urls": StringF,
        "username": StringOrNull,
        "credential": StringOrNull,
      },
      throwable: true,
    );

    return IceServer(
      urls: map["urls"],
      username: map["username"] ?? "",
      credential: map["credential"] ?? "",
    );
  }

  factory IceServer.fromYaml(YamlMap yaml) {
    return IceServer.fromMap(yaml.cast());
  }

  @override
  String toString() {
    return "IceServer: urls->'$urls', username->'$username', credential->'$credential'";
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
