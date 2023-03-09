import 'dart:async';
import 'dart:convert';
import 'dart:io';
import 'package:apn/config.dart';
import 'package:apn/routes.dart';
import 'package:dieklingel_core_shared/mqtt/mqtt_response.dart';
import 'package:http/http.dart' as http;
import 'package:yaml/yaml.dart';
import 'package:interpolation/interpolation.dart';

// TODO(KoiFresh): import dart_shared
import 'hive/registry_entry_adapter.dart';
import 'models/registry_entry.dart';
import 'package:dieklingel_core_shared/blocs/mqtt_client_bloc.dart';
import 'package:dieklingel_core_shared/models/mqtt_uri.dart';
import 'package:hive/hive.dart';
import 'package:path/path.dart' as p;

class Apn {
  late MqttUri uri;
  late Uri worker;
  late String title;
  late String body;

  MqttClientBloc mqtt = MqttClientBloc();

  void main() async {
    stdout.writeln("APN: App Push Notification");

    //Directory directory = await getApplicationDocumentsDirectory();
    String path = p.join(
      Platform.environment["HOME"] ?? "",
      "dieklingel",
    );
    Hive.init(path);
    Hive.registerAdapter(RegistryEntryAdapter());
    await Hive.openBox<RegistryEntry>("apn");

    await setupMqttChannels();

    YamlMap config = await getConfig();
    uri = MqttUri.fromUri(
      Uri.parse(
        config["mqtt"]?["uri"] ?? "mqtt://127.0.0.1:1883/com.dieklingel/",
      ),
    );
    // TODO: create error message, if key does not exists
    worker = Uri.parse(config["apn"]?["url"]);
    title = config["apn"]?["title"] ?? "no title :(";
    body = config["apn"]?["body"] ?? "no body :(";

    mqtt.uri.add(uri);
  }

  Future<void> setupMqttChannels() async {
    mqtt.watch("/io/action/sign/clicked").listen((event) async {
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

    mqtt.answer("request/apn/register/+", registerToken);
    mqtt.answer("request/apn/delete/+", deleteToken);
    mqtt.answer("request/apn/list/+", listTokens);
  }

  Future<void> sendPushNotification(List<RegistryEntry> entries) async {
    if (entries.isEmpty) {
      return;
    }

    DateTime time = DateTime.now();
    Interpolation interpolation = Interpolation();

    String title = interpolation.eval(this.title, {
      "YEAR": time.year.toString(),
      "MONTH": time.month.toString().padLeft(2, "0"),
      "DAY": time.day.toString().padLeft(2, "0"),
      "HOUR": time.hour.toString().padLeft(2, "0"),
      "MINUTE": time.minute.toString().padLeft(2, "0"),
      "SECOND": time.second.toString().padLeft(2, "0"),
    });

    String body = interpolation.eval(this.body, {
      "YEAR": time.year.toString(),
      "MONTH": time.month.toString().padLeft(2, "0"),
      "DAY": time.day.toString().padLeft(2, "0"),
      "HOUR": time.hour.toString().padLeft(2, "0"),
      "SECOND": time.second.toString().padLeft(2, "0"),
    });

    Map<String, dynamic> payload = {
      "tokens": entries.map((e) => e.token).toList(),
      "title": title,
      "body": body,
      "image": "",
    };

    // TODO(KoiFresh): delete entries, if response is not ok
    await http.post(worker, body: jsonEncode(payload));
    // TODO: log response
  }
}
