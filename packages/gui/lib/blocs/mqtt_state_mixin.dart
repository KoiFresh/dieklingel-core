import 'dart:async';

import 'package:dieklingel_core_shared/shared.dart';

import 'stream_event.dart';
import '../utils/mqtt_channel_constants.dart';
import 'package:get_it/get_it.dart';

mixin MqttStateMixin {
  Stream<ActivityState> get activity {
    return GetIt.I<MqttClientBloc>().watch(kIoActivityState).map(
      (event) {
        return event.value.toLowerCase().trim() == "inactive"
            ? InactiveState()
            : ActiveState();
      },
    );
  }

  Stream<DisplayState> get display {
    return GetIt.I<MqttClientBloc>().watch(kIoDisplayState).map(
      (event) {
        return event.value.toLowerCase().trim() == "off"
            ? DisplayOffState()
            : DisplayOnState();
      },
    );
  }
}
