import 'dart:io';

import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:mqtt/mqtt.dart';

import 'blocs/app_view_bloc.dart';
import 'controllers/controller_manager.dart';
import 'repositories/action_repository.dart';
import 'repositories/app_repository.dart';
import 'repositories/device_repository.dart';
import 'repositories/ice_server_repository.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:mqtt/mqtt.dart' as mqtt;
import 'package:path/path.dart' as path;

import 'repositories/sign_repository.dart';
import 'views/app_view.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  String homeDirectory = Platform.environment["SNAP_REAL_HOME"] ??
      Platform.environment["HOME"] ??
      "";
  Directory.current = path.join(homeDirectory, "dieklingel");

  final actionRepository = ActionRepository();
  final appRepository = AppRepository();
  final iceServerRepository = IceServerRepository();
  final signRepository = SignRepository();
  final deviceRepository = DeviceRepository();

  mqtt.Client client = mqtt.Client();

  client
      .connect(
        MqttUri.fromUri(await appRepository.fetchMqttUri()),
        username: await appRepository.fetchMqttUsername(),
        password: await appRepository.fetchMqttPassword(),
      )
      .then(
        (value) =>
            client.publish(Message("system/boot", DateTime.now().toString())),
      );

  final controllerManager = ControllerManager(
    client,
    iceServerRepository,
    actionRepository,
    deviceRepository,
  );

  runApp(
    MultiProvider(
      providers: [
        Provider(create: (_) => controllerManager),
        RepositoryProvider(create: (_) => appRepository),
        RepositoryProvider(create: (_) => actionRepository),
        RepositoryProvider(create: (_) => signRepository),
        BlocProvider(create: (_) => AppViewBloc(appRepository, client))
      ],
      child: const App(),
    ),
  );
}
