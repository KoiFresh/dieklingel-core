import 'package:flutter/material.dart';
import 'package:flutter_webrtc/flutter_webrtc.dart';
import 'package:rtc/models/ice_server.dart';
import 'package:rtc/signaling/signaling_message.dart';
import 'package:rtc/utils/media_ressource.dart';

import '../signaling/signaling_message_type.dart';
import 'rtc_transceiver.dart';

class RtcClientWrapper {
  final MediaRessource ressource = MediaRessource();
  final RTCVideoRenderer renderer = RTCVideoRenderer();
  final List<IceServer> servers;
  final List<RtcTransceiver> transceivers;
  late final RTCPeerConnection connection;

  void Function(SignalingMessage)? _onMessage;

  RtcClientWrapper._(this.servers, this.transceivers);

  static Future<RtcClientWrapper> create({
    List<IceServer> iceServers = const [],
    List<RtcTransceiver> transceivers = const [],
  }) async {
    WidgetsFlutterBinding.ensureInitialized();
    RtcClientWrapper wrapper = RtcClientWrapper._(iceServers, transceivers);
    await wrapper.renderer.initialize();

    List<Map<String, dynamic>> servers = iceServers
        .map(
          (e) => {
            "urls": e.urls,
            "username": e.username,
            "credential": e.credential
          },
        )
        .toList();

    Map<String, dynamic> configuration = {};
    configuration["iceServers"] = servers;
    configuration["sdpSemantics"] = "unified-plan";
    wrapper.connection = await createPeerConnection(configuration);

    wrapper.connection
      ..onIceCandidate = wrapper._onIceCandidate
      ..onConnectionState = wrapper._onConnectionState
      ..onTrack = wrapper._onTrack;

    for (RtcTransceiver transceiver in transceivers) {
      await wrapper.connection.addTransceiver(
        kind: transceiver.kind,
        init: RTCRtpTransceiverInit(direction: transceiver.direction),
      );
    }

    return wrapper;
  }

  void addMessage(SignalingMessage message) async {
    switch (message.type) {
      case SignalingMessageType.offer:
        MediaStream? stream = ressource.stream;
        if (null != stream) {
          for (MediaStreamTrack track in stream.getTracks()) {
            await connection.addTrack(track, stream);
          }
        }

        final remote = RTCSessionDescription(
          message.data["sdp"],
          message.data["type"],
        );

        await connection.setRemoteDescription(remote);
        final local = await connection.createAnswer();
        await connection.setLocalDescription(local);

        SignalingMessage answer = SignalingMessage()
          ..type = SignalingMessageType.answer
          ..data = local.toMap();

        _onMessage?.call(answer);
        break;
      case SignalingMessageType.answer:
        final description = RTCSessionDescription(
          message.data["sdp"],
          message.data["type"],
        );

        connection.setRemoteDescription(description);
        break;
      case SignalingMessageType.candidate:
        print("candiddate");
        final candidate = RTCIceCandidate(
          message.data["candidate"],
          message.data["sdpMid"],
          message.data["sdpMLineIndex"],
        );

        connection.addCandidate(candidate);
        break;
      case SignalingMessageType.leave:
      case SignalingMessageType.busy:
      case SignalingMessageType.error:
        close();
        break;
    }
  }

  void onMessage(void Function(SignalingMessage) handler) {
    _onMessage = handler;
  }

  Future<void> close() async {
    SignalingMessage message = SignalingMessage()
      ..type = SignalingMessageType.leave;

    _onMessage?.call(message);
    await dispose();
  }

  Future<void> dispose() async {
    await connection.close();
    ressource.close();
  }

  void _onIceCandidate(RTCIceCandidate candidate) {
    print("candidate-");
    SignalingMessage message = SignalingMessage()
      ..type = SignalingMessageType.candidate
      ..data = candidate.toMap();

    _onMessage?.call(message);
  }

  void _onConnectionState(RTCPeerConnectionState state) {
    print(state);
  }

  void _onTrack(RTCTrackEvent event) {
    if (event.streams.isEmpty) {
      return;
    }

    renderer.srcObject = event.streams.first;
  }
}
