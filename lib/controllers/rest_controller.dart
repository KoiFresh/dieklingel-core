import 'dart:async';
import 'dart:convert';

import 'package:mqtt/models/legacy_mqtt_response.dart';
import 'package:mqtt/models/mqtt_uri.dart';
import 'package:mqtt/models/request.dart';
import 'package:mqtt/mqtt.dart';

import '../repositories/action_repository.dart';
import '../repositories/ice_server_repository.dart';
import '../signaling/signaling_message.dart';
import '../signaling/signaling_message_type.dart';
import '../utils/mqtt_channel.dart';
import '../utils/rtc_client_wrapper.dart';

class RestController {
  final Map<StreamSubscription, RtcClientWrapper> connections = {};

  final Client client;
  final IceServerRepository iceServerRepository;
  final ActionRepository actionRepository;

  RestController(this.client, this.iceServerRepository, this.actionRepository) {
    client.answer("request/rtc/connect", _onRtcConnectionRequest);
    client.answer("rest/snapshot", _onSnapshotRequest);
    client.answer("rest/action/trigger", _onActionTrigger);

    client.legacyAnswer("request/rtc/connect/+", _onLegacyRtcConnectionRequest);
  }

  Future<Response> _onSnapshotRequest(Request request) async {
    return Response.notImplemented;
  }

  Future<Response> _onRtcConnectionRequest(Request request) async {
    return Response.notImplemented;
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

  Future<Response> _onActionTrigger(Request request) async {
    return Response.notImplemented;
  }
}
