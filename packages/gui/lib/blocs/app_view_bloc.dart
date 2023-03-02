import 'dart:async';

import 'package:dieklingel_core_shared/flutter_shared.dart';
import 'package:gui/blocs/mqtt_state_mixin.dart';
import 'package:gui/blocs/stream_event.dart';
import 'package:gui/config.dart';
import 'package:hive_flutter/hive_flutter.dart';
import 'package:rxdart/rxdart.dart';
import 'package:flutter/painting.dart';

class AppViewBloc extends Bloc with MqttStateMixin {
  final _clip = BehaviorSubject<EdgeInsets>();
  final _timeout = BehaviorSubject<Duration>();
  final _activity = BehaviorSubject<ActivityState>();

  Stream<EdgeInsets> get clip => _clip.stream;

  Stream<Duration> get timeout => _timeout.stream;

  AppViewBloc() {
    Box settings = Hive.box("settings");

    _clip.add(EdgeInsets.fromLTRB(
      settings.get(kSettingsGuiViewportClipLeft, defaultValue: 0.0),
      settings.get(kSettingsGuiViewportClipTop, defaultValue: 0.0),
      settings.get(kSettingsGuiViewportClipRight, defaultValue: 0.0),
      settings.get(kSettingsGuiViewportClipBottom, defaultValue: 0.0),
    ));

    settings.watch(key: kSettingsGuiViewportClipLeft).listen((event) {
      EdgeInsets insets = _clip.value.copyWith(left: event.value);
      _clip.add(insets);
    });
    settings.watch(key: kSettingsGuiViewportClipTop).listen((event) {
      EdgeInsets insets = _clip.value.copyWith(top: event.value);
      _clip.add(insets);
    });
    settings.watch(key: kSettingsGuiViewportClipRight).listen((event) {
      EdgeInsets insets = _clip.value.copyWith(right: event.value);
      _clip.add(insets);
    });
    settings.watch(key: kSettingsGuiViewportClipBottom).listen((event) {
      EdgeInsets insets = _clip.value.copyWith(bottom: event.value);
      _clip.add(insets);
    });

    _timeout.add(
      Duration(
        seconds: settings.get(
          kSettingsGuiScreensaverTimeout,
          defaultValue: 30,
        ),
      ),
    );

    settings.watch(key: kSettingsGuiScreensaverTimeout).listen((event) {
      Duration duration = Duration(seconds: event.value);
      _timeout.add(duration);
    });
  }

  @override
  void dispose() {
    _clip.close();
  }
}
