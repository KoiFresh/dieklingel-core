import 'package:sqflite/sqflite.dart';

import '../models/device.dart';

import 'database_repository.dart';

class DeviceRepository extends DatabaseRepository {
  Future<List<Device>> fetchAllDevices() async {
    Database db = await open();
    List<Map<String, dynamic>> entries = await db.query("devices");
    List<Device> devices = [];

    for (Map<String, dynamic> entry in entries) {
      String token = entry["token"];
      List<Map<String, dynamic>> signs = await db.query(
        "signs",
        where: "device_token = ?",
        whereArgs: [token],
      );
      devices.add(
        Device(
          signs: signs.map((e) => e["name"].toString()).toList(),
          token: token,
        ),
      );
    }

    await db.close();
    return devices;
  }

  Future<Device> fetchDeviceByToken(String token) async {
    Database db = await open();
    List<Map<String, dynamic>> entries = await db.query(
      "devices",
      where: "token = ?",
      whereArgs: [token],
      limit: 1,
    );

    if (entries.isEmpty) {
      throw FetchDeviceError("The requested device does not exist.", token);
    }

    List<Map<String, dynamic>> signs = await db.query(
      "signs",
      where: "device_token = ?",
      whereArgs: [token],
    );

    Device device = Device(
      signs: signs.map((e) => e["name"].toString()).toList(),
      token: token,
    );

    await db.close();
    return device;
  }

  Future<Device> addDevice(Device device) async {
    Database db = await open();

    await db.insert(
      "devices",
      {
        "token": device.token,
      },
    );

    for (String sign in device.signs) {
      await db.insert(
        "signs",
        {
          "name": sign,
          "device_token": device.token,
        },
      );
    }

    await db.close();
    return device;
  }

  Future<Device> modifyDevice(Device device) async {
    Database db = await open();

    List<Map<String, dynamic>> entries = await db.query(
      "devices",
      where: "token = ?",
      whereArgs: [device.token],
      limit: 1,
    );

    if (entries.isEmpty) {
      throw ModifyDeviceError(
          "The Device can not be modified because it does not exist.", device);
    }

    await db.delete(
      "signs",
      where: "device_token = ?",
      whereArgs: [device.token],
    );

    for (String sign in device.signs) {
      await db.insert(
        "signs",
        {
          "name": sign,
          "device_token": device.token,
        },
      );
    }

    await db.close();
    return device;
  }

  Future<void> deleteDevice(Device device) async {
    Database db = await open();

    await db.delete(
      "signs",
      where: "device_token = ?",
      whereArgs: [device.token],
    );
    await db.delete(
      "devices",
      where: "token = ?",
      whereArgs: [device.token],
    );

    await db.close();
  }
}

class ModifyDeviceError extends Error {
  final String message;
  final Device? device;

  ModifyDeviceError(this.message, [this.device]);

  @override
  String toString() {
    return "$message, requested device: $device";
  }
}

class FetchDeviceError extends Error {
  final String message;
  final String? token;

  FetchDeviceError(this.message, [this.token]);

  @override
  String toString() {
    return "$message, token: $token";
  }
}
