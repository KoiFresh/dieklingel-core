import 'dart:io';

import 'package:flutter_bloc/flutter_bloc.dart';

import 'controllers/rest_controller.dart';
import 'repositories/action_repository.dart';
import 'repositories/ice_server_repository.dart';
import 'package:flutter/material.dart';
import 'package:mqtt/models/mqtt_uri.dart';
import 'package:provider/provider.dart';
import 'package:yaml/yaml.dart';
import 'package:mqtt/mqtt.dart' as mqtt;
import 'package:path/path.dart' as p;

import 'views/app_view.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();

  String path = p.relative("/usr/share/dieklingel/core.yaml");
  String raw = await File(path).readAsString();
  YamlMap config = await loadYaml(raw);

  mqtt.Client client = mqtt.Client();
  client.connect(
    MqttUri.parse(config["mqtt"]["uri"]),
    username: config["mqtt"]["username"],
    password: config["mqtt"]["password"],
  );

  IceServerRepository iceServerRepository = IceServerRepository();
  ActionRepository actionRepository = ActionRepository();

  runApp(
    MultiProvider(
      providers: [
        Provider(
          create: (_) => RestController(
            client,
            iceServerRepository,
            actionRepository,
          ),
          lazy: false,
        ),
        RepositoryProvider(create: (_) => actionRepository)
      ],
      child: const App(),
    ),
  );
}
