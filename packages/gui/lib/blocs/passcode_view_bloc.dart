import 'package:dieklingel_core_shared/flutter_shared.dart';
import 'package:get_it/get_it.dart';
import 'package:gui/utils/mqtt_channel_constants.dart';

class PasscodeViewBloc extends Bloc {
  void sendPasscode(String code) {
    if (code.isEmpty) {
      return;
    }
    MqttClientBloc mqtt = GetIt.I<MqttClientBloc>();
    mqtt.message.add(ChannelMessage(kIoActionUnlockPasscode, code));
  }

  @override
  void dispose() {
    // Noting to do, no streams yet
  }
}
