import 'package:yaml/yaml.dart';

class MqttUri {
  final String host;
  final int port;
  final String channel;
  final String section;
  final bool ssl;
  final bool websocket;

  MqttUri({
    this.host = "",
    this.port = 1883,
    String channel = "",
    this.section = "",
    this.ssl = true,
    this.websocket = false,
  }) : channel =
            channel.split('/').where((element) => element.isNotEmpty).join('/');

  factory MqttUri.fromMap(Map<String, dynamic> map) {
    if (map["host"] is! String) {
      throw FormatException(
        "The key 'host' with a value of type String does not exist.",
        map,
      );
    }

    if (map["port"] is! int) {
      throw FormatException(
        "The key 'port' with a value of type int does not exist.",
        map,
      );
    }

    if (map["channel"] != null && map["channel"] is! String) {
      throw FormatException(
        "The key 'channel' has either to be of null, of type String or not existing.",
        map,
      );
    }

    if (map["section"] != null && map["section"] is! String) {
      throw FormatException(
        "The key 'section' has either to be of null, of type String or not existing.",
        map,
      );
    }

    if (map["ssl"] != null && map["ssl"] is! bool) {
      throw FormatException(
        "The key 'ssl' has either to be of null, of type bool or not existing.",
        map,
      );
    }

    if (map["websocket"] != null && map["websocket"] is! bool) {
      throw FormatException(
        "The key 'websocket' has either to be of null, of type bool or not existing.",
        map,
      );
    }

    return MqttUri(
      host: map["host"],
      port: map["port"],
      channel: map["channel"] ?? "",
      section: map["section"] ?? "",
      ssl: map["ssl"] ?? true,
      websocket: map["websocket"] ?? false,
    );
  }

  factory MqttUri.fromUri(Uri uri) {
    return MqttUri(
      host: uri.host,
      port: uri.port,
      channel: uri.path.isEmpty ? uri.path : uri.path.substring(1),
      section: uri.fragment,
      ssl: uri.scheme == "mqtts" || uri.scheme == "wss",
      websocket: uri.scheme == "ws" || uri.scheme == "wss",
    );
  }

  factory MqttUri.parse(String uri) {
    return MqttUri.fromUri(Uri.parse(uri));
  }

  Map<String, dynamic> toMap() {
    return {
      "host": host,
      "port": port,
      "channel": channel,
      "section": section,
      "ssl": ssl,
      "websocket": websocket,
    };
  }

  Uri toUri() {
    String scheme = websocket
        ? ssl
            ? "wss"
            : "ws"
        : ssl
            ? "mqtts"
            : "mqtt";
    return Uri(
      host: host,
      port: port,
      path: channel,
      fragment: section,
      scheme: scheme,
    );
  }

  @override
  bool operator ==(Object other) {
    if (other is! MqttUri) {
      return false;
    }
    return host == other.host &&
        port == other.port &&
        channel == other.channel &&
        section == other.section &&
        ssl == other.ssl &&
        websocket == other.websocket;
  }

  MqttUri copy() {
    return copyWith();
  }

  MqttUri copyWith({
    String? host,
    int? port,
    String? channel,
    String? section,
    bool? ssl,
    bool? websocket,
  }) {
    return MqttUri(
      host: host ?? this.host,
      port: port ?? this.port,
      channel: channel ?? this.channel,
      section: section ?? this.section,
      ssl: ssl ?? this.ssl,
      websocket: websocket ?? this.websocket,
    );
  }

  @override
  String toString() {
    return "host: $host; port: $port; channel: $channel; section: $section; ssl: $ssl; websocket: $websocket";
  }

  @override
  int get hashCode => Object.hash(host, port, channel, section, ssl, websocket);
}
