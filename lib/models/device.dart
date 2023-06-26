import 'package:blueprint/blueprint.dart';

class Device {
  final String token;
  final List<String> signs;

  Device({
    required this.signs,
    required this.token,
  });

  factory Device.fromMap(Map<String, dynamic> map) {
    matchMap(
      map,
      {
        "token": StringF,
        "signs": ListF.of(StringF),
      },
      throwable: true,
    );

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
