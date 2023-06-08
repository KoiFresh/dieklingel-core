class Device {
  final String token;
  final List<String> signs;

  Device({
    required this.signs,
    required this.token,
  });

  factory Device.fromMap(Map<String, dynamic> map) {
    if (map["signs"] is! List) {
      throw FormatException(
        "In order to create a Device from a map, the map has to have a key 'signs' of type String.",
        map,
      );
    }

    if (map["token"] is! String) {
      throw FormatException(
        "In order to create a Device from a map, the map has to have a key 'token' of type String.",
        map,
      );
    }

    return Device(
      signs: (map["signs"] as List).cast<String>(),
      token: map["token"],
    );
  }

  Map<String, dynamic> toMap() {
    return {
      "signs": signs,
      "token": token,
    };
  }
}
