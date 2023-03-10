import 'package:hive_flutter/hive_flutter.dart';

import '../models/sign_options.dart';

class SignOptionsAdapter extends TypeAdapter<SignOptions> {
  @override
  SignOptions read(BinaryReader reader) {
    Map<String, dynamic> map = reader.readMap().cast<String, dynamic>();
    SignOptions uri = SignOptions.fromMap(map);
    return uri;
  }

  @override
  int get typeId => 1;

  @override
  void write(BinaryWriter writer, SignOptions obj) {
    Map<dynamic, dynamic> map = obj.toMap();
    writer.writeMap(map);
  }
}
