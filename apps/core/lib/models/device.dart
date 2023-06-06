class Device {
  final String id;
  final List<String> signs;

  Device({
    required this.id,
    required this.signs,
  });

  factory Device.fromMap(Map<String, dynamic> map) {
    if (map["id"] is! String) {
      throw FormatException(
        "In order to create a Device from a map, the map has to have a key 'id' of type String.",
        map,
      );
    }

    if (map["signs"] is! List<String>) {
      throw FormatException(
        "In order to create a Device from a map, the map has to have a key 'signs' of type String.",
        map,
      );
    }

    return Device(
      id: map["id"],
      signs: map["signs"],
    );
  }

  Map<String, dynamic> toMap() {
    return {
      "id": id,
      "signs": signs,
    };
  }
}
