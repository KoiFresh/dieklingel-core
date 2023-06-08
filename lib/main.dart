import 'dart:io';

import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:get_it/get_it.dart';

import 'blocs/app_view_bloc.dart';
import 'repositories/action_repository.dart';
import 'repositories/app_repository.dart';
import 'repositories/device_repository.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:mqtt/mqtt.dart';
import 'package:path/path.dart' as path;
import 'package:shelf/shelf_io.dart' as io;

import 'repositories/sign_repository.dart';
import 'services/authentication_service.dart';
import 'services/camera_service.dart';
import 'services/action_service.dart';
import 'services/device_service.dart';
import 'views/app_view.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  String homeDirectory = Platform.environment["SNAP_REAL_HOME"] ??
      Platform.environment["HOME"] ??
      "";
  Directory.current = path.join(homeDirectory, "dieklingel");

  GetIt.I.registerSingleton(ActionRepository());
  GetIt.I.registerSingleton(AppRepository());
  GetIt.I.registerSingleton(SignRepository());
  GetIt.I.registerSingleton(DeviceRepository());

  final service = AuthenticationService({
    "/actions": ActionService(GetIt.I<ActionRepository>()).handler,
    "/camera": CameraService().handler,
    "/devices": DeviceService(GetIt.I<DeviceRepository>()).handler,
  });

  /* final actionRepository = ActionRepository();
  final appRepository = AppRepository();
  final iceServerRepository = IceServerRepository();
  final signRepository = SignRepository();
  final deviceRepository = DeviceRepository();*/

  await Future.wait([
    io.serve(service.handler, "0.0.0.0", 8081),
    MqttHttpServer().serve(
      service.handler,
      Uri.parse("mqtt://server.dieklingel.com:1883/dieklingel"),
    )
  ]);

  runApp(
    MultiProvider(
      providers: [
        BlocProvider(create: (_) => AppViewBloc(GetIt.I<AppRepository>()))
      ],
      child: const App(),
    ),
  );
}
