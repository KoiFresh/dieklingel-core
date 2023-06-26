import 'package:hive/hive.dart';

import '../hive/hive_device_adapter.dart';
import '../models/device.dart';

import 'database_repository.dart';

class DeviceRepository extends DatabaseRepository {
  DeviceRepository() {
    if (!Hive.isAdapterRegistered(HiveDeviceAdapter().typeId)) {
      Hive.registerAdapter(HiveDeviceAdapter());
    }
  }

  Future<List<Device>> fetchAllDevices({List<String>? signs}) async {
    Box<Device> deviceBox = await Hive.openBox("devices");

    List<Device> devices = deviceBox.values.toList();
    if (signs != null) {
      devices.removeWhere(
        (device) => device.signs.every(
          (sign) => !signs.contains(sign),
        ),
      );
    }

    await deviceBox.compact();
    await deviceBox.close();
    return devices;
  }

  Future<Device> fetchDeviceByToken(String token) async {
    Box<Device> deviceBox = await Hive.openBox("devices");

    Device? device = deviceBox.get(token);
    if (device == null) {
      throw FetchDeviceError("The requested Device could not be found.", token);
    }

    await deviceBox.compact();
    await deviceBox.close();
    return device;
  }

  Future<Device> addDevice(Device device) async {
    Box<Device> deviceBox = await Hive.openBox("devices");

    if (deviceBox.containsKey(device.token)) {
      throw DeviceError(
        "The Device could not be created, the token already exists!",
        device,
      );
    }
    await deviceBox.put(device.token, device);

    await deviceBox.compact();
    await deviceBox.close();
    return device;
  }

  Future<Device> modifyDevice(String oldToken, Device device) async {
    Box<Device> deviceBox = await Hive.openBox("devices");

    if (!deviceBox.containsKey(oldToken)) {
      throw DeviceError(
        "The Device can not be modified, it has to be created first",
        device,
      );
    }
    if (oldToken != device.token) {
      await deviceBox.delete(oldToken);
    }
    await deviceBox.put(device.token, device);

    await deviceBox.compact();
    await deviceBox.close();
    return device;
  }

  Future<void> deleteDevice(Device device) async {
    Box<Device> deviceBox = await Hive.openBox("devices");

    await deviceBox.delete(device.token);

    await deviceBox.compact();
    await deviceBox.close();
  }
}

class DeviceError extends Error {
  final String message;
  final Device? device;

  DeviceError(this.message, [this.device]);

  @override
  String toString() {
    return "$message; device: $device";
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
