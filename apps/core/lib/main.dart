import 'dart:io';

import 'package:flutter_bloc/flutter_bloc.dart';

import 'blocs/app_view_bloc.dart';
import 'controllers/rest_controller.dart';
import 'repositories/action_repository.dart';
import 'repositories/app_repository.dart';
import 'repositories/device_repository.dart';
import 'repositories/ice_server_repository.dart';
import 'package:flutter/material.dart';
import 'package:mqtt/models/mqtt_uri.dart';
import 'package:provider/provider.dart';
import 'package:mqtt/mqtt.dart' as mqtt;
import 'package:path/path.dart' as p;

import 'repositories/sign_repository.dart';
import 'views/app_view.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  String homeDirectory = Platform.environment["SNAP_REAL_HOME"] ??
      Platform.environment["HOME"] ??
      "";
  Directory.current = p.join(homeDirectory, "dieklingel");

  ActionRepository actionRepository = ActionRepository();
  AppRepository appRepository = AppRepository();
  IceServerRepository iceServerRepository = IceServerRepository();
  SignRepository signRepository = SignRepository();
  DeviceRepository deviceRepository = DeviceRepository();

  mqtt.Client client = mqtt.Client();

  client.connect(
    MqttUri.fromUri(await appRepository.fetchMqttUri()),
    username: await appRepository.fetchMqttUsername(),
    password: await appRepository.fetchMqttPassword(),
  );

  runApp(
    MultiProvider(
      providers: [
        Provider(
          create: (_) => RestController(
            client,
            iceServerRepository,
            actionRepository,
            deviceRepository,
          ),
          lazy: false,
        ),
        RepositoryProvider(create: (_) => appRepository),
        RepositoryProvider(create: (_) => actionRepository),
        RepositoryProvider(create: (_) => signRepository),
        BlocProvider(
          create: (_) => AppViewBloc(appRepository, client),
          lazy: false,
        )
      ],
      child: const App(),
    ),
  );
}
