import 'dart:convert';

import 'package:mqtt/mqtt.dart';
import 'package:shelf/shelf.dart';
import 'package:shelf_router/shelf_router.dart';

import '../repositories/ice_server_repository.dart';
import '../signaling/signaling_message.dart';
import '../signaling/signaling_message_type.dart';
import '../utils/rtc_client_wrapper.dart';

class RtcService {
  final IceServerRepository iceServerRepository;
  final Map<String, RtcClientWrapper> _connections = {};

  RtcService(this.iceServerRepository);

  Handler get handler {
    final Router router = Router();

    router.get("/connections", (Request request) async {
      List<Map<String, dynamic>> connections = [];

      for (MapEntry<String, RtcClientWrapper> entry in _connections.entries) {
        connections.add({
          "id": entry.key,
          "connectionState": entry.value.connection.connectionState.toString()
        });
      }

      return Response.ok(
        jsonEncode(connections),
        headers: {
          "Content-Type": "application/json",
        },
      );
    });

    router.post("/connections/<channel|.*>", (
      Request request,
      String channel,
    ) async {
      if (_connections.containsKey(channel)) {
        return Response.notFound(
          "The requested connection could not be created, because it is already in use.",
        );
      }

      RtcClientWrapper wrapper = await RtcClientWrapper.create(
        iceServers: await iceServerRepository.fetchAllIceServers(),
      );

      MqttHttpClient client = MqttHttpClient();

      wrapper.onMessage(
        (SignalingMessage message) {
          if (message.type == SignalingMessageType.leave ||
              message.type == SignalingMessageType.error) {
            wrapper.dispose();
            _connections.remove(channel);
          }

          client.socket(
            request.requestedUri.replace(
              path: "/$channel",
            ),
            headers: {
              "Content-Type": "application/json",
            },
            body: jsonEncode(message.toJson()),
          );
        },
      );

      _connections[channel] = wrapper;

      await wrapper.ressource.open(true, true);

      return Response(
        201,
        headers: {
          "Content-Type": "application/json",
        },
      );
    });

    router.connect("/connections/<channel>", (
      Request request,
      String channel,
    ) async {
      RtcClientWrapper? wrapper = _connections[channel];
      Map<String, dynamic> body;

      if (wrapper == null) {
        return Response.notFound(
          "The requested channel '$channel' was not found.",
        );
      }

      body = jsonDecode(await request.readAsString());
      SignalingMessage message = SignalingMessage.fromJson(body);
      wrapper.addMessage(message);

      return Response(200);
    });

    return router;
  }
}
