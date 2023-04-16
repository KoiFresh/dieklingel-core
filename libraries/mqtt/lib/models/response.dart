class Response {
  static const Response ok = Response(status: 200, message: "Ok");

  final int status;
  final String message;
  final dynamic body;

  const Response({
    required this.status,
    required this.message,
    this.body,
  });

  factory Response.fromMap(Map<String, dynamic> map) {
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

    return Response(
      status: map["status"],
      message: map["message"],
      body: map["body"],
    );
  }
}
