import 'package:flutter/material.dart';

class AppState {
  final DisplayState display;
  final EdgeInsets clip;

  AppState({
    this.display = DisplayState.off,
    this.clip = EdgeInsets.zero,
  });

  AppState copyWith({
    DisplayState? display,
    EdgeInsets? clip,
  }) {
    return AppState(
      display: display ?? this.display,
      clip: clip ?? this.clip,
    );
  }
}

class DisplayState {
  static const DisplayState on = DisplayState._(true);
  static const DisplayState off = DisplayState._(false);

  final bool isOn;

  bool get isOff => !isOn;

  const DisplayState._(this.isOn);
}
