import 'package:dieklingel_core_shared/dart_shared.dart';
import 'package:dieklingel_core_shared/mqtt/mqtt_response.dart';

class MqttRequest {
  MqttRequest._();

  static Future<MqttResponse> get(
    MqttUri uri,
    String channel,
    String message, {
    String? username,
    String? password,
    Duration timeout = const Duration(seconds: 30),
  }) async {
    MqttClientBloc mqtt = MqttClientBloc();

    if (username != null) {
      mqtt.usernanme.add(username);
    }
    if (password != null) {
      mqtt.password.add(password);
    }

    Stopwatch stopwatch = Stopwatch()..start();
    MqttClientState state = await mqtt.state
        .firstWhere((state) => state == MqttClientState.connected)
        .timeout(
          timeout,
          onTimeout: () => MqttClientState.disconnected,
        );
    stopwatch.stop();
    if (state != MqttClientState.connected) {
      return const MqttResponse(
        status: 599,
        message:
            "Timeout before a connection to the mqtt broker could be established.",
      );
    }

    Duration remaining = timeout - stopwatch.elapsed;
    await mqtt.disconnect();
    return mqtt.request(channel, message, timeout: remaining);
  }
}
