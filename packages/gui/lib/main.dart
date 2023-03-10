import 'dart:io';

import 'package:dieklingel_core_shared/flutter_shared.dart';
import 'package:flutter/material.dart';
import 'package:get_it/get_it.dart';
import 'package:gui/blocs/app_state_bloc.dart';
import 'package:gui/blocs/app_view_bloc.dart';
import 'package:gui/blocs/stream_event.dart';
import 'package:gui/config.dart';
import 'package:gui/utils/mqtt_channel_constants.dart';
import 'views/app_view.dart';

void main() async {
  stdout.writeln("GUI: Graphical User Interface");
  WidgetsFlutterBinding.ensureInitialized();
  await Config.read();

  GetIt.I.registerSingleton(MqttClientBloc());
  GetIt.I.registerSingleton(AppStateBloc());

  await setupMqttChannels();
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

  bloc.watch(kIoActivityState).listen((event) {
    if (Config.get<bool>(kSettingsGuiScreensaverEnabled) ?? false) {
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
    if (Config.get<bool>(kSettingsGuiScreensaverEnabled) ?? false) {
      if (message.toLowerCase().trim() == "off") {
        return "off";
      } else if (message.toLowerCase().trim() == "on") {
        return "on";
      }
    }

    return null;
  });
}

Future<void> connect() async {
  MqttClientBloc bloc = GetIt.I<MqttClientBloc>();

  String? username = Config.get<String>(kSettingsMqttUsername);
  if (username != null) {
    bloc.usernanme.add(username);
  }

  String? password = Config.get<String>(kSettingsMqttPassword);
  if (password != null) {
    bloc.password.add(password);
  }

  String? uri = Config.get<String>(kSettingsMqttUri);
  if (uri == null) {
    stderr.writeln(
      "$kSettingsMqttUri is not specified in config.yaml! The gui package will not work like expected!",
    );
    return;
  }

  try {
    bloc.uri.add(MqttUri.fromUri(Uri.parse(uri)));
  } catch (exception) {
    stderr.writeln(exception.toString());
  }
}
