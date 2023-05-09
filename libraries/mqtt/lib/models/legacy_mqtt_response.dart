class LegacyMqttResponse {
  static const LegacyMqttResponse ok =
      LegacyMqttResponse(status: 200, message: "Ok");

  final int status;
  final String message;
  final dynamic body;

  const LegacyMqttResponse({
    required this.status,
    required this.message,
    this.body,
  });

  factory LegacyMqttResponse.fromMap(Map<String, dynamic> map) {
    if (map["status"] is! int) {
      throw Exception(
        "Cannot create a MqttResponse from map without a status field of type 'int'.",
      );
    }
    if (map["message"] is! String) {
      throw Exception(
        "Cannot create a MqttResponse from map without a message field of type 'String'.",
      );
    }

    return LegacyMqttResponse(
      status: map["status"],
      message: map["message"],
      body: map["body"],
    );
  }

  LegacyMqttResponse copyWith({
    int? status,
    String? message,
    dynamic body,
  }) {
    return LegacyMqttResponse(
      status: status ?? this.status,
      message: message ?? this.message,
      body: body ?? this.body,
    );
  }

  Map<String, dynamic> toMap() {
    return {
      "status": status,
      "message": message,
      "body": body,
    };
  }

  @override
  String toString() {
    return "MqttResponse: status: $status, message: $message, body: $body";
  }
}
