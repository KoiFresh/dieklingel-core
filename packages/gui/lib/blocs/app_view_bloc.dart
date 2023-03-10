import 'dart:async';

import 'package:dieklingel_core_shared/flutter_shared.dart';
import 'package:gui/blocs/mqtt_state_mixin.dart';
import 'package:gui/blocs/stream_event.dart';
import 'package:gui/config.dart';
import 'package:rxdart/rxdart.dart';
import 'package:flutter/painting.dart';

class AppViewBloc extends Bloc with MqttStateMixin {
  final _clip = BehaviorSubject<EdgeInsets>();
  final _timeout = BehaviorSubject<Duration>();
  final _activity = BehaviorSubject<ActivityState>();

  Stream<EdgeInsets> get clip => _clip.stream;

  Stream<Duration> get timeout => _timeout.stream;

  AppViewBloc() {
    _clip.add(EdgeInsets.fromLTRB(
      Config.get<double>(kSettingsGuiViewportClipLeft) ?? 0.0,
      Config.get<double>(kSettingsGuiViewportClipTop) ?? 0.0,
      Config.get<double>(kSettingsGuiViewportClipRight) ?? 0.0,
      Config.get<double>(kSettingsGuiViewportClipBottom) ?? 0.0,
    ));

    _timeout.add(
      Duration(seconds: Config.get<int>(kSettingsGuiScreensaverTimeout) ?? 30),
    );
  }

  @override
  void dispose() {
    _clip.close();
  }
}
