class Request {
  final Uri location;
  final String body;

  const Request({
    required this.location,
    this.body = "",
  });

  factory Request.fromMap(Map<String, dynamic> map) {
    Uri uri;
    dynamic body;

    if (map["location"] is! String) {
      throw FormatException(
        "The map is missing the 'location' propertie.",
        map,
      );
    }

    uri = Uri.parse(map["location"]);

    if (map["body"] == null) {
      throw FormatException(
        "The map is missing the propertie.",
        map,
      );
    }

    body = map["body"];

    return Request(
      location: uri,
      body: body,
    );
  }
}
