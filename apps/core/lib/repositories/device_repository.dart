import 'dart:convert';
import 'dart:io';

import '../models/device.dart';
import 'base_repository.dart';

import 'package:path/path.dart' as path;

class DeviceRepository extends BaseRepository {
  Future<List<Device>> fetchAllDevices() async {
    File deviceFile = File(path.join(homePath, "devices.json"));
    if (!(await deviceFile.exists())) {
      return [];
    }

    String rawDevices = await deviceFile.readAsString();
    List<dynamic> jsonDevices = jsonDecode(rawDevices);

    return jsonDevices.map((e) => Device.fromMap(e)).toList();
  }

  Future<void> addDevice(Device device) async {}
}
