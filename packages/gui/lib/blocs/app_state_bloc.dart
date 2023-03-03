import 'dart:async';

import 'package:dieklingel_core_shared/flutter_shared.dart';
import 'package:get_it/get_it.dart';
import 'package:gui/blocs/stream_event.dart';
import 'package:gui/config.dart';
import 'package:gui/utils/mqtt_channel_constants.dart';
import 'package:hive_flutter/hive_flutter.dart';
import 'package:rxdart/subjects.dart';

class AppStateBloc extends Bloc {
  Timer? _timer;
  ActivityState _activityState = ActiveState();
  final _activity = BehaviorSubject<ActivityState>.seeded(ActiveState());
  final _display = BehaviorSubject<DisplayState>.seeded(DisplayOnState());

  StreamController<ActivityState> get activity => _activity;

  AppStateBloc() {
    GetIt.I<MqttClientBloc>().watch(kIoActivityState).map(
      (event) {
        return event.value.toLowerCase().trim() == "inactive"
            ? InactiveState()
            : ActiveState();
      },
    ).listen((event) {
      _activity.add(event);
    });

    _activity.listen((value) {
      if (value is ActiveState) {
        _timer?.cancel();
        _timer = Timer(_duration, () {
          _timer?.cancel();
          _activity.add(InactiveState());
        });
      }
      if (value is InactiveState) {
        _timer?.cancel();
      }
      if (value is ActiveState && _activityState is InactiveState ||
          value is InactiveState && _activityState is ActiveState) {
        GetIt.I<MqttClientBloc>().message.add(
              ChannelMessage(
                kIoActivityState,
                value.toString(),
              ),
            );
      }
      _activityState = value;
    });
  }

  Duration get _duration {
    Box settings = Hive.box("settings");
    int seconds = settings.get(
      kSettingsGuiScreensaverTimeout,
      defaultValue: 30,
    );
    return Duration(seconds: seconds);
  }

  @override
  void dispose() {
    _timer?.cancel();
    _activity.close();
  }
}
