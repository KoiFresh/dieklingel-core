import 'dart:async';
import 'dart:convert';

import 'package:dieklingel_core_shared/shared.dart';
import 'package:rtc/signaling/signaling_message.dart';
import 'package:rtc/signaling/signaling_message_type.dart';
import 'package:rtc/utils/mqtt_channel.dart';
import 'package:rtc/utils/rtc_client_wrapper.dart';

class App {
  MqttUri uri = MqttUri(
    host: "server.dieklingel.com",
    port: 1883,
    channel: "com.dieklingel/mayer/kai/",
  );
  MqttClientBloc mqtt = MqttClientBloc();
  Map<StreamSubscription, RtcClientWrapper> connections = {};

  Future<void> main() async {
    mqtt.uri.add(uri);
    await setup();
  }

  Future<void> setup() async {
    mqtt.answer("request/rtc/+", (String message) async {
      // TODO(KoiFresh): setup from yaml
      List<IceServer> ice = [
        IceServer(urls: "stun:stun1.l.google.com:19302"),
        IceServer(urls: "stun:relay.metered.ca:80"),
        IceServer(
          urls: "turn:relay.metered.ca:80",
          username: "b2d44a5253ab2ddd58522b34",
          credential: "zeFyJa3y04YHpYs6",
        ),
        IceServer(
          urls: "turn:relay.metered.ca:443",
          username: "b2d44a5253ab2ddd58522b34",
          credential: "zeFyJa3y04YHpYs6",
        ),
      ];

      RtcClientWrapper wrapper = await RtcClientWrapper.create(iceServers: ice);
      MqttUri connUri = MqttUri.fromMap(jsonDecode(message));

      MqttChannel channel = MqttChannel(connUri.channel.toString()).remove(
        MqttChannel(uri.channel.toString()),
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
          if (message.type == SignalingMessageType.leave ||
              message.type == SignalingMessageType.error) {
            subscription.cancel();
            wrapper.dispose();

            connections.remove(subscription);
          }

          mqtt.message.add(
            ChannelMessage(
              answer,
              jsonEncode(message.toJson()),
            ),
          );
        },
      );

      // TODO(KoiFresh): open audio in snap crashes, so we disable audio for now
      await wrapper.ressource.open(false, true);

      connections[subscription] = wrapper;
      return "OK";
    });
  }
}
