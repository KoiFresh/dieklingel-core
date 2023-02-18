import 'dart:async';
import 'dart:convert';

import 'package:rtc/blocs/mqtt_client_bloc.dart';
import 'package:rtc/models/mqtt_uri.dart';
import 'package:rtc/signaling/signaling_message.dart';
import 'package:rtc/utils/mqtt_channel.dart';
import 'package:rtc/utils/rtc_client_wrapper.dart';

class App {
  MqttUri uri = MqttUri(
    host: "server.dieklingel.com",
    port: 1883,
    channel: "com.dieklingel/test/",
  );
  MqttClientBloc mqtt = MqttClientBloc();
  Map<StreamSubscription, RtcClientWrapper> connections = {};

  Future<void> main() async {
    mqtt.uri.add(uri);
  }

  Future<void> setup() async {
    mqtt.answer("request/rtc/+", (String message) async {
      RtcClientWrapper wrapper = await RtcClientWrapper.create();
      MqttUri uri = MqttUri.fromMap(jsonDecode(message));

      MqttChannel channel = MqttChannel(uri.channel).remove(
        MqttChannel(uri.toString()),
      );

      String invite = channel.append("invite").toString();
      String answer = channel.append("answer").toString();

      StreamSubscription subscription = mqtt.watch(invite).listen(
        (event) {
          SignalingMessage message = SignalingMessage.fromJson(
            jsonDecode(event.value),
          );

          wrapper.addMessage(message);
        },
      );

      wrapper.onMessage(
        (SignalingMessage message) {
          mqtt.message.add(
            ChannelMessage(
              answer,
              jsonEncode(message.toJson()),
            ),
          );
        },
      );

      connections[subscription] = wrapper;
      return "OK";
    });
  }
}
