import 'package:dieklingel_core/handlers/handler.dart';
import 'package:mqtt/mqtt.dart' as mqtt;

class MqttHandler implements Handler {
  final mqtt.Client client = mqtt.Client();

  MqttHandler();
  @override
  void run() {}
}
