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

  factory IceServer.fromYaml(YamlMap yaml) {
    return IceServer(
      urls: yaml["urls"] ?? "",
      username: yaml["username"] ?? "",
      credential: yaml["credential"] ?? "",
    );
  }

  @override
  String toString() {
    return "urls: $urls; username: $username; credential: $credential";
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
