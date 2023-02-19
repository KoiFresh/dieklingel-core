import 'dart:ffi';
import 'dart:io';
import 'dart:isolate';

import 'package:flutter/material.dart';
import 'package:get_it/get_it.dart';
import 'package:gui/bloc/bloc_provider.dart';
import 'package:gui/bloc/multi_bloc_provider.dart';
import 'package:gui/blocs/app_view_bloc.dart';
import 'package:hive_flutter/hive_flutter.dart';
import 'package:yaml/yaml.dart';

import 'blocs/mqtt_client_bloc.dart';
import 'models/mqtt_uri.dart';
import 'models/sign_options.dart';
import 'views/app_view.dart';

import 'package:rtc/rtc.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();

  RtcPackage.main([]);

  GetIt.I.registerSingleton(MqttClientBloc());

  await Hive.initFlutter();
  Hive
    ..registerAdapter(MqttUriAdapter())
    ..registerAdapter(SignOptionsAdapter());

  await Future.wait([
    Hive.openBox<SignOptions>((SignOptions).toString()),
    Hive.openBox("settings"),
  ]);

  await configure();
  await setup();
  await connect();

  runApp(
    MultiBlocProvider(
      blocs: [
        BlocProvider(bloc: GetIt.I<MqttClientBloc>()),
        BlocProvider(bloc: AppViewBloc()),
      ],
      child: const MyApp(),
    ),
  );
}

Future<void> configure() async {
  MqttClientBloc bloc = GetIt.I<MqttClientBloc>();

  bloc.answer("request/test/+", (message) async {
    return "Ok";
  });

  Box settings = Hive.box("settings");

  bloc.filter("display/state", (String message) {
    if (settings.get("screensaver.enabled") as bool) {
      if (message.toLowerCase().trim() == "off") {
        return "off";
      } else if (message.toLowerCase().trim() == "on") {
        return "on";
      }
    }

    return null;
  });
}

Future<void> setup() async {
  YamlMap config = await getConfig();
  Box settings = Hive.box("settings");

  settings.put(
    "mqtt.uri",
    MqttUri.fromUri(
      Uri.parse(
        config["mqtt"]?["uri"] ?? "mqtt://127.0.0.1:1883/com.dieklingel/",
      ),
    ),
  );

  settings.put("mqtt.username", config["mqtt"]?["username"] ?? "");
  settings.put("mqtt.password", config["mqtt"]?["password"] ?? "");

  settings.put(
    "viewport.clip.left",
    double.parse(
      config["viewport"]?["clip"]?["left"]?.toString() ?? "0",
    ),
  );

  settings.put(
    "viewport.clip.top",
    double.parse(
      config["viewport"]?["clip"]?["top"]?.toString() ?? "0",
    ),
  );

  settings.put(
    "viewport.clip.right",
    double.parse(
      config["viewport"]?["clip"]?["right"]?.toString() ?? "0",
    ),
  );

  settings.put(
    "viewport.clip.bottom",
    double.parse(
      config["viewport"]?["clip"]?["bottom"]?.toString() ?? "0",
    ),
  );

  settings.put(
    "screensaver.enabled",
    config["screensaver"]?["enabled"] as bool? ?? true,
  );

  settings.put(
    "screensaver.file",
    config["screensaver"]?["file"] as String? ?? "",
  );

  await SignOptions.boxx.clear();
  for (YamlMap sign in config["signs"] ?? []) {
    SignOptions options;

    try {
      options = SignOptions.fromYaml(sign);
    } catch (exception) {
      stdout.writeln("Skip Sign: $exception");
      continue;
    }

    await options.save();
  }
}

Future<void> connect() async {
  Box settings = Hive.box("settings");
  MqttClientBloc bloc = GetIt.I<MqttClientBloc>();

  bloc.usernanme.add(settings.get("mqtt.username"));
  bloc.password.add(settings.get("mqtt.password"));
  bloc.uri.add(settings.get("mqtt.uri"));
}

Future<YamlMap> getConfig() async {
  YamlMap result = YamlMap();
  try {
    final configFile = File("/etc/dieklingel/config.yaml");
    result = await loadYaml(
      await configFile.readAsString(),
    );
  } catch (exception) {
    // could not load, do nothing
  }
  return result;
}
