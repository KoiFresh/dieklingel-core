class RegistryEntry {
  String token;
  String identifier;
  DateTime created;

  RegistryEntry({
    required this.token,
    required this.identifier,
  }) : created = DateTime.now();

  factory RegistryEntry.fromMap(Map<String, dynamic> map) {
    if (!map.containsKey("token")) {
      throw "Cannot create RegistryEntry from map without token";
    }

    if (!map.containsKey("identifier")) {
      throw "Cannot create RegistryEntry from map without identifier";
    }

    RegistryEntry entry = RegistryEntry(
      token: map["token"],
      identifier: map["identifier"],
    );

    if (map.containsKey("created")) {
      entry.created = DateTime.parse(map["created"]);
    }

    return entry;
  }

  Map<String, dynamic> toMap() {
    return {
      "token": token,
      "identifier": identifier,
      "created": created.toIso8601String(),
    };
  }

  @override
  operator ==(Object other) {
    if (other is! RegistryEntry) {
      return false;
    }
    return other.identifier == identifier && other.token == token;
  }

  @override
  String toString() {
    return "token: $token; identifier: $identifier; created: ${created.toIso8601String()}";
  }

  @override
  int get hashCode => Object.hash(identifier, token);
}
