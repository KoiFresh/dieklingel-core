import 'dart:convert';

import 'package:dieklingel_core_shared/mqtt/mqtt_response.dart';
import 'package:hive/hive.dart';

import 'models/registry_entry.dart';

Future<MqttResponse> registerToken(String message) async {
  Box<RegistryEntry> box = Hive.box<RegistryEntry>("apn");

  RegistryEntry entry;
  try {
    entry = RegistryEntry.fromMap(jsonDecode(message));
  } catch (exception) {
    return MqttResponse(status: 400, message: exception.toString());
  }

  for (dynamic key in box.keys) {
    if (entry == box.get(key)) {
      box.delete(key);
      break;
    }
  }
  box.add(entry);

  return MqttResponse.ok;
}

Future<MqttResponse> deleteToken(String message) async {
  Box<RegistryEntry> box = Hive.box<RegistryEntry>("apn");

  RegistryEntry entry;
  try {
    entry = RegistryEntry.fromMap(jsonDecode(message));
  } catch (exception) {
    return MqttResponse(status: 400, message: exception.toString());
  }

  for (dynamic key in box.keys) {
    if (entry == box.get(key)) {
      box.delete(key);
      break;
    }
  }

  return MqttResponse.ok;
}

Future<MqttResponse> listTokens(String message) async {
  Box<RegistryEntry> box = Hive.box<RegistryEntry>("apn");
  List<Map<String, dynamic>> entries =
      box.values.map((e) => e.toMap()).toList();

  return MqttResponse.ok.copyWith(body: entries);
}
