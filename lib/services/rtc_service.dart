import 'dart:async';
import 'dart:convert';

import 'package:dieklingel_core/models/ice_server.dart';
import 'package:flutter/material.dart';
import 'package:mqtt/models/mqtt_uri.dart';
import 'package:mqtt/mqtt.dart';

import '../signaling/signaling_message.dart';
import '../signaling/signaling_message_type.dart';
import '../utils/rtc_client_wrapper.dart';

class RTCService extends ChangeNotifier {
  final Client client;
  final List<IceServer> iceServers;
  final Map<StreamSubscription, RtcClientWrapper> connections = {};

  RTCService(this.client, this.iceServers) {
    client.answer("request/rtc/connect/+", _onRtcConnectionRequest);
  }

  Future<Message> _onRtcConnectionRequest(Message message) async {
    final wrapper = await RtcClientWrapper.create(iceServers: iceServers);

    MqttUri uri = MqttUri.fromMap(jsonDecode(message.payload));

    String invite = MqttUri(channel: "${uri.channel}/invite").channel;
    String answer = MqttUri(channel: "${uri.channel}/answer").channel;

    StreamSubscription subscription = client.watch(invite).listen(
      (event) {
        final message = SignalingMessage.fromJson(jsonDecode(event.payload));
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

        client.publish(Message(answer, jsonEncode(message.toJson())));
      },
    );

    await wrapper.ressource.open(true, true);

    connections[subscription] = wrapper;
    return Message(
      MqttUri(channel: "${message.topic}/response").channel,
      jsonEncode(Response.ok.toMap()),
    );
  }
}
