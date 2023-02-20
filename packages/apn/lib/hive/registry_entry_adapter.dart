import 'package:apn/models/registry_entry.dart';
import 'package:hive/hive.dart';

class RegistryEntryAdapter extends TypeAdapter<RegistryEntry> {
  @override
  RegistryEntry read(BinaryReader reader) {
    Map<String, dynamic> map = reader.readMap().cast<String, dynamic>();
    RegistryEntry entry = RegistryEntry.fromMap(map);
    return entry;
  }

  @override
  int get typeId => 1;

  @override
  void write(BinaryWriter writer, RegistryEntry obj) {
    Map<dynamic, dynamic> map = obj.toMap();
    writer.writeMap(map);
  }
}
