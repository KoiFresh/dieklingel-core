import 'dart:convert';
import 'dart:io';
import 'package:http/http.dart' as http;

// TODO(KoiFresh): import dart_shared
import 'hive/registry_entry_adapter.dart';
import 'models/registry_entry.dart';
import 'package:dieklingel_core_shared/blocs/mqtt_client_bloc.dart';
import 'package:dieklingel_core_shared/models/mqtt_uri.dart';
import 'package:hive/hive.dart';
import 'package:path/path.dart' as p;

class Apn {
  // TODO(KoiFresh): read from config file
  MqttUri uri = MqttUri(
    host: "server.dieklingel.com",
    port: 1883,
    channel: "com.dieklingel/mayer/kai/",
  );
  MqttClientBloc mqtt = MqttClientBloc();

  void main() async {
    stdout.writeln("APN: App Push Notification");
    mqtt.uri.add(uri);

    //Directory directory = await getApplicationDocumentsDirectory();
    String path = p.join(
      Platform.environment["HOME"] ?? "",
      "dieklingel",
    );
    Hive.init(path);
    Hive.registerAdapter(RegistryEntryAdapter());
    await Hive.openBox<RegistryEntry>("apn");

    mqtt.watch("/io/action/sign/clicked").listen((event) async {
      String topic = event.key;
      String message = event.value;

      Box<RegistryEntry> box = Hive.box<RegistryEntry>("apn");

      DateTime obsolte = DateTime.now().subtract(Duration(days: 60));

      List<RegistryEntry> entries = box.values.toList();
      entries.removeWhere((element) => element.created.isBefore(obsolte));
      await box.clear();
      await box.addAll(entries);
      await box.flush();

      List<RegistryEntry> registries = entries
          .where(
            (element) => element.identifier == message,
          )
          .toList();

      sendPushNotification(registries);
    });

    mqtt.answer("request/apn/register", (String message) async {
      Box<RegistryEntry> box = Hive.box<RegistryEntry>("apn");

      RegistryEntry entry;
      try {
        entry = RegistryEntry.fromMap(jsonDecode(message));
      } catch (exception) {
        return "ERROR";
      }

      for (dynamic key in box.keys) {
        if (entry == box.get(key)) {
          box.delete(key);
          break;
        }
      }
      box.add(entry);

      return "OK";
    });
  }

  Future<void> sendPushNotification(List<RegistryEntry> entries) async {
    Uri url = Uri.https("fcm-worker.dieklingel.workers.dev", "/fcm/send");

    // TODO(KoiFresh): allow interpolation with title and body
    Map<String, dynamic> payload = {
      "tokens": entries.map((e) => e.token).toList(),
      "title": "New Notification!",
      "body": "Someone has rang",
      "image": "",
    };

    // TODO(KoiFresh): delete entries, if response is not ok
    await http.post(url, body: jsonEncode(payload));
  }
}
