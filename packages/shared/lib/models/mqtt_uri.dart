class MqttUri {
  final String host;
  final int port;
  final String channel;
  final bool ssl;
  final bool websocket;

  MqttUri({
    required this.host,
    required this.port,
    this.channel = "",
    this.ssl = true,
    this.websocket = false,
  });

  factory MqttUri.fromMap(Map<String, dynamic> json) {
    return MqttUri(
      host: json["host"],
      port: json["port"],
      channel: json["channel"],
      ssl: json["ssl"].toString() != "false",
      websocket: json["websocket"].toString() == "true",
    );
  }

  factory MqttUri.fromUri(Uri uri) {
    return MqttUri(
      host: uri.host,
      port: uri.port,
      channel: uri.path.substring(1),
      ssl: uri.scheme == "mqtts" || uri.scheme == "wss",
      websocket: uri.scheme == "ws" || uri.scheme == "wss",
    );
  }

  Map<String, dynamic> toMap() {
    return {
      "host": host,
      "port": port,
      "channel": channel,
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
    bool? ssl,
    bool? websocket,
  }) {
    return MqttUri(
      host: host ?? this.host,
      port: port ?? this.port,
      channel: channel ?? this.channel,
      ssl: ssl ?? this.ssl,
      websocket: websocket ?? this.websocket,
    );
  }

  @override
  String toString() {
    return "host: $host; port: $port; channel: $channel; ssl: $ssl; websocket: $websocket";
  }

  @override
  int get hashCode => Object.hash(host, port, channel, ssl, websocket);
}
