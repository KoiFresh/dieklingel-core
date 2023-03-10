import 'dart:io';

import 'package:dieklingel_core_shared/flutter_shared.dart';
import 'package:flutter/material.dart';
import 'package:get_it/get_it.dart';
import 'package:gui/blocs/app_state_bloc.dart';
import 'package:gui/blocs/app_view_bloc.dart';
import 'package:gui/blocs/stream_event.dart';
import 'package:gui/config.dart';
import 'package:gui/hive/mqtt_uri_adapter.dart';
import 'package:gui/hive/sign_options_adapter.dart';
import 'package:gui/utils/mqtt_channel_constants.dart';
import 'package:hive_flutter/hive_flutter.dart';
import 'package:yaml/yaml.dart';

import 'models/sign_options.dart';
import 'views/app_view.dart';

void main() async {
  stdout.writeln("GUI: Graphical User Interface");
  WidgetsFlutterBinding.ensureInitialized();

  await Hive.initFlutter();
  Hive
    ..registerAdapter(MqttUriAdapter())
    ..registerAdapter(SignOptionsAdapter());

  await Future.wait([
    Hive.openBox<SignOptions>((SignOptions).toString()),
    Hive.openBox("gui_settings"),
  ]);

  GetIt.I.registerSingleton(MqttClientBloc());
  GetIt.I.registerSingleton(AppStateBloc());

  await setupMqttChannels();
  await setupConfigFile();
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

Future<void> setupMqttChannels() async {
  MqttClientBloc bloc = GetIt.I<MqttClientBloc>();

  Box settings = Hive.box("gui_settings");

  bloc.watch(kIoActivityState).listen((event) {
    if (!(settings.get(kSettingsGuiScreensaverEnabled) as bool)) {
      return;
    }

    String message = event.value;

    if (message.toLowerCase().trim() == ActiveState().toString()) {
      bloc.message.add(
        ChannelMessage(
          kIoDisplayState,
          DisplayOnState().toString(),
        ),
      );
    } else if (message.toLowerCase().trim() == InactiveState().toString()) {
      bloc.message.add(
        ChannelMessage(
          kIoDisplayState,
          DisplayOffState().toString(),
        ),
      );
    }
  });

  bloc.filter(kIoDisplayState, (String message) {
    if (settings.get(kSettingsGuiScreensaverEnabled) as bool) {
      if (message.toLowerCase().trim() == "off") {
        return "off";
      } else if (message.toLowerCase().trim() == "on") {
        return "on";
      }
    }

    return null;
  });
}

Future<void> setupConfigFile() async {
  YamlMap config = await getConfig();
  Box settings = Hive.box("gui_settings");

  settings.put(
    kSettingsMqttUri,
    MqttUri.fromUri(
      Uri.parse(
        config["mqtt"]?["uri"] ?? "mqtt://127.0.0.1:1883/com.dieklingel/",
      ),
    ),
  );

  settings.put(kSettingsMqttUsername, config["mqtt"]?["username"] ?? "");
  settings.put(kSettingsMqttPassword, config["mqtt"]?["password"] ?? "");

  settings.put(
    kSettingsGuiViewportClipLeft,
    double.parse(
      config["gui"]?["viewport"]?["clip"]?["left"]?.toString() ?? "0",
    ),
  );

  settings.put(
    kSettingsGuiViewportClipTop,
    double.parse(
      config["gui"]?["viewport"]?["clip"]?["top"]?.toString() ?? "0",
    ),
  );

  settings.put(
    kSettingsGuiViewportClipRight,
    double.parse(
      config["gui"]?["viewport"]?["clip"]?["right"]?.toString() ?? "0",
    ),
  );

  settings.put(
    kSettingsGuiViewportClipBottom,
    double.parse(
      config["gui"]?["viewport"]?["clip"]?["bottom"]?.toString() ?? "0",
    ),
  );

  settings.put(
    kSettingsGuiScreensaverTimeout,
    config["gui"]?["screensaver"]?["timeout"] as int? ?? 30,
  );

  settings.put(
    kSettingsGuiScreensaverEnabled,
    config["gui"]?["screensaver"]?["enabled"] as bool? ?? true,
  );

  settings.put(
    kSettingsGuiScreensaverFile,
    config["gui"]?["screensaver"]?["file"] as String? ?? "",
  );

  await SignOptions.boxx.clear();
  for (YamlMap sign in config["gui"]?["signs"] ?? []) {
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
  Box settings = Hive.box("gui_settings");
  MqttClientBloc bloc = GetIt.I<MqttClientBloc>();

  bloc.usernanme.add(settings.get(kSettingsMqttUsername));
  bloc.password.add(settings.get(kSettingsMqttPassword));
  bloc.uri.add(settings.get(kSettingsMqttUri));
}
