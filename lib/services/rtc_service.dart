import 'dart:async';
import 'dart:convert';

import 'package:dieklingel_core/models/ice_server.dart';
import 'package:mqtt/models/request.dart';
import 'package:mqtt/mqtt.dart';
import 'package:path/path.dart' as p;

import '../signaling/signaling_message.dart';
import '../signaling/signaling_message_type.dart';
import '../utils/rtc_client_wrapper.dart';

class RTCService {
  final Client client;
  final List<IceServer> iceServers;
  final Map<StreamSubscription, RtcClientWrapper> connections = {};

  RTCService(this.client, this.iceServers) {
    client.answer("request/rtc/connect", _onRtcConnectionRequest);
  }

  Future<Response> _onRtcConnectionRequest(Request request) async {
    final wrapper = await RtcClientWrapper.create(iceServers: iceServers);

    Uri uri = Uri.parse(request.body);

    String invite = p.normalize("${uri.path}/invite");
    String answer = p.normalize("${uri.path}/answer");

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
    return Response.ok;
  }
}
