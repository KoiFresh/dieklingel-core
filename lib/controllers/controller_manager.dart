import 'dart:async';
import 'dart:convert';

import 'package:mqtt/models/legacy_mqtt_response.dart';
import 'package:mqtt/mqtt.dart';

import '../repositories/action_repository.dart';
import '../repositories/device_repository.dart';
import '../repositories/ice_server_repository.dart';
import '../signaling/signaling_message.dart';
import '../signaling/signaling_message_type.dart';
import '../utils/mqtt_channel.dart';
import '../utils/rtc_client_wrapper.dart';
import 'base_controller.dart';
import 'device_controller.dart';
import 'rtc_controlller.dart';
import 'snapshot_controller.dart';

class ControllerManager {
  final Map<StreamSubscription, RtcClientWrapper> connections = {};
  final List<BaseController> _controllers = [];

  final Client client;
  final IceServerRepository iceServerRepository;
  final ActionRepository actionRepository;

  ControllerManager(
    this.client,
    this.iceServerRepository,
    this.actionRepository,
    DeviceRepository deviceRepository,
  ) {
    client.legacyAnswer("request/rtc/connect/+", _onLegacyRtcConnectionRequest);

    _controllers
      ..add(DeviceController(client, deviceRepository))
      ..add(RtcController(client))
      ..add(SnapshotController(client));
  }

  Future<LegacyMqttResponse> _onLegacyRtcConnectionRequest(
    String message,
  ) async {
    RtcClientWrapper wrapper = await RtcClientWrapper.create(
      iceServers: await iceServerRepository.fetchAllIceServers(),
    );
    MqttUri connUri = MqttUri.fromMap(jsonDecode(message));

    MqttChannel channel = MqttChannel(connUri.channel.toString()).remove(
      MqttChannel(client.prefix ?? ""),
    );

    String invite = channel.append("invite").toString();
    String answer = channel.append("answer").toString();

    StreamSubscription subscription = client.watch(invite).listen(
      (event) {
        SignalingMessage message = SignalingMessage.fromJson(
          jsonDecode(event.payload),
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

        client.publish(
          Message(
            answer,
            jsonEncode(message.toJson()),
          ),
        );
      },
    );

    await wrapper.ressource.open(true, true);

    connections[subscription] = wrapper;
    return LegacyMqttResponse.ok;
  }
}
