import 'dart:async';

import 'package:dieklingel_core_shared/flutter_shared.dart';
import 'package:gui/config.dart';
import 'package:hive_flutter/hive_flutter.dart';
import 'package:rxdart/rxdart.dart';
import 'package:flutter/painting.dart';

class AppViewBloc extends Bloc {
  final _clip = BehaviorSubject<EdgeInsets>();

  Stream<EdgeInsets> get clip => _clip.stream;

  AppViewBloc() {
    Box settings = Hive.box("settings");

    _clip.add(EdgeInsets.fromLTRB(
      settings.get(kSettingsGuiViewportClipLeft, defaultValue: 0.0),
      settings.get(kSettingsGuiViewportClipTop, defaultValue: 0.0),
      settings.get(kSettingsGuiViewportClipRight, defaultValue: 0.0),
      settings.get(kSettingsGuiViewportClipBottom, defaultValue: 0.0),
    ));

    settings.watch(key: "viewport.clip.left").listen((event) {
      EdgeInsets insets = _clip.value.copyWith(left: event.value);
      _clip.add(insets);
    });
    settings.watch(key: "viewport.clip.top").listen((event) {
      EdgeInsets insets = _clip.value.copyWith(top: event.value);
      _clip.add(insets);
    });
    settings.watch(key: "viewport.clip.right").listen((event) {
      EdgeInsets insets = _clip.value.copyWith(right: event.value);
      _clip.add(insets);
    });
    settings.watch(key: "viewport.clip.bottom").listen((event) {
      EdgeInsets insets = _clip.value.copyWith(bottom: event.value);
      _clip.add(insets);
    });
  }

  @override
  void dispose() {
    _clip.close();
  }
}
