import 'package:hive/hive.dart';

import '../models/device.dart';

class HiveDeviceAdapter extends TypeAdapter<Device> {
  @override
  Device read(BinaryReader reader) {
    Map<dynamic, dynamic> map = reader.readMap();

    return Device.fromMap(map.cast<String, dynamic>());
  }

  @override
  int get typeId => 1;

  @override
  void write(BinaryWriter writer, Device obj) {
    writer.writeMap(obj.toMap());
  }
}
