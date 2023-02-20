import 'package:dieklingel_core_shared/flutter_shared.dart';
import 'package:hive_flutter/hive_flutter.dart';
import 'package:rxdart/rxdart.dart';

import '../models/screensaver_options.dart';

class ScreensaverViewBloc extends Bloc {
  final _file = BehaviorSubject<String>.seeded("");

  Stream<ScreensaverOptions> get options => _file.stream.map(
        (event) => ScreensaverOptions(file: event),
      );

  ScreensaverViewBloc() {
    Box settings = Hive.box("settings");

    _file.add(settings.get("screensaver.file"));
    settings.watch(key: "screensaver.file").listen((event) {
      _file.add(event.value);
    });
  }

  @override
  void dispose() {
    _file.close();
  }
}
