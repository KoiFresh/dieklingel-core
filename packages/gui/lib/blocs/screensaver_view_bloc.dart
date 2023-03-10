import 'dart:io';

import 'package:dieklingel_core_shared/flutter_shared.dart';
import 'package:gui/config.dart';
import 'package:rxdart/rxdart.dart';

import '../models/screensaver_options.dart';

class ScreensaverViewBloc extends Bloc {
  final _file = BehaviorSubject<String>.seeded("");

  Stream<ScreensaverOptions> get options => _file.stream.map(
        (event) => ScreensaverOptions(file: event),
      );

  ScreensaverViewBloc() {
    String? file = Config.get<String>(kSettingsGuiScreensaverFile);
    bool enabled = Config.get<bool>(kSettingsGuiScreensaverEnabled) ?? false;
    if (!enabled) {
      return;
    }
    if (file == null) {
      stderr.writeln(
        "The Screensaver wil not work as excpected, because $kSettingsGuiScreensaverFile is not specified, but $kSettingsGuiScreensaverEnabled is true",
      );
      return;
    }

    _file.add(file);
  }

  @override
  void dispose() {
    _file.close();
  }
}
