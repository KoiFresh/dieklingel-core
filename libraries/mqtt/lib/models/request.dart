class Request {
  final Uri request;
  final Uri response;
  final String method;
  final dynamic body;

  const Request({
    required this.request,
    required this.response,
    this.method = "GET",
    this.body,
  });

  const Request.get({
    required this.request,
    required this.response,
    this.body,
  }) : method = "GET";

  const Request.post({
    required this.request,
    required this.response,
    this.body,
  }) : method = "POST";

  factory Request.fromMap(Map<String, dynamic> map) {
    Uri request;
    Uri response;
    Map<String, String> headers;
    String method;
    dynamic body;

    if (map["request"] is! String) {
      throw FormatException(
        "The map is missing the 'request' propertie.",
        map,
      );
    }
    request = Uri.parse(map["request"]);

    if (map["response"] is! String) {
      throw FormatException(
        "The map is missing the 'response' propertie.",
        map,
      );
    }
    response = Uri.parse(map["response"]);

    if (map["method"] is! String) {
      throw FormatException(
        "The map is missing the 'method' propertie.",
        map,
      );
    }
    method = map["method"];

    /* if (map["body"] == null) {
      throw FormatException(
        "The map is missing the 'body' propertie.",
        map,
      );
    }*/
    body = map["body"];

    return Request(
      request: request,
      response: response,
      method: method,
      body: body,
    );
  }

  Map<String, dynamic> toMap() {
    return {
      "request": request.path,
      "response": response.path,
      "method": method,
      "body": body,
    };
  }
}
