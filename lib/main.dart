import 'dart:io';

import 'package:blueprint/blueprint.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:get_it/get_it.dart';
import 'package:logger/logger.dart';
import 'package:window_manager/window_manager.dart';

import 'repositories/ice_server_repository.dart';
import 'services/rtc_service.dart';
import 'ui/blocs/app_view_bloc.dart';
import 'repositories/action_repository.dart';
import 'repositories/app_repository.dart';
import 'repositories/device_repository.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:mqtt/mqtt.dart';
import 'package:shelf/shelf_io.dart' as io;

import 'repositories/sign_repository.dart';
import 'services/authentication_service.dart';
import 'services/camera_service.dart';
import 'services/action_service.dart';
import 'services/device_service.dart';
import 'services/sign_service.dart';
import 'ui/views/app_view.dart';

void main(List<String> args) async {
  WidgetsFlutterBinding.ensureInitialized();

  if (Platform.environment["DIEKLINGEL_HOME"] != null) {
    Directory.current = Platform.environment["DIEKLINGEL_HOME"];
  }

  if (args.contains("--kiosk") || args.contains("-k")) {
    Logger.info("Running as kiosk.");

    WindowManager.instance
      ..setFullScreen(true)
      ..setAlwaysOnTop(true)
      ..setAsFrameless();
  }

  GetIt.I
    ..registerSingleton(ActionRepository())
    ..registerSingleton(AppRepository())
    ..registerSingleton(SignRepository())
    ..registerSingleton(DeviceRepository())
    ..registerSingleton(IceServerRepository());

  try {
    await GetIt.I<AppRepository>().validate(throwable: true);
  } on BluePrintException catch (exception) {
    Logger.error(exception.msg);
    exit(1);
  }

  final service = AuthenticationService({
    "/actions": ActionService(GetIt.I<ActionRepository>()).handler,
    "/camera": CameraService().handler,
    "/devices": DeviceService(GetIt.I<DeviceRepository>()).handler,
    "/rtc": RtcService(GetIt.I<IceServerRepository>()).handler,
    "/signs": SignService(GetIt.I<SignRepository>()).handler,
  });

  int port = await GetIt.I<AppRepository>().fetchHttpPort();

  MqttHttpServer server = MqttHttpServer();

  Future.wait([
    io.serve(service.handler, "0.0.0.0", port),
    server.serve(
      service.handler,
      await GetIt.I<AppRepository>().fetchMqttUri(),
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
