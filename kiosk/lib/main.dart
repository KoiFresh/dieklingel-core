import 'dart:io';

import 'package:blueprint/blueprint.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:get_it/get_it.dart';
import 'package:logger/logger.dart';
import 'package:window_manager/window_manager.dart';

import 'ui/blocs/app_view_bloc.dart';
import 'repositories/app_repository.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import 'repositories/sign_repository.dart';
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
    ..registerSingleton(AppRepository())
    ..registerSingleton(SignRepository());

  try {
    await GetIt.I<AppRepository>().validate(throwable: true);
  } on BluePrintException catch (exception) {
    Logger.error(exception.msg);
    exit(1);
  }

  runApp(
    MultiProvider(
      providers: [
        BlocProvider(create: (_) => AppViewBloc(GetIt.I<AppRepository>()))
      ],
      child: const App(),
    ),
  );
}
