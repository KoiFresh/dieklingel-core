import 'package:flutter_webrtc/flutter_webrtc.dart';
import 'package:rtc/models/ice_server.dart';
import 'package:rtc/signaling/signaling_message.dart';
import 'package:rtc/utils/media_ressource.dart';

import 'rtc_transceiver.dart';

class RtcClientWrapper {
  final MediaRessource ressource = MediaRessource();
  final RTCVideoRenderer renderer = RTCVideoRenderer()..initialize();
  final List<IceServer> servers;
  final List<RtcTransceiver> transceivers;
  late final RTCPeerConnection connection;

  void Function(SignalingMessage)? _onMessage;

  RtcClientWrapper._(this.servers, this.transceivers);

  static Future<RtcClientWrapper> create({
    List<IceServer> iceServers = const [],
    List<RtcTransceiver> transceivers = const [],
  }) async {
    RtcClientWrapper wrapper = RtcClientWrapper._(iceServers, transceivers);
    //TODO(KoiFresh): use ice servers in configuration
    Map<String, dynamic> configuration = {};
    wrapper.connection = await createPeerConnection(configuration);

    return wrapper;
  }

  void addMessage(SignalingMessage message) {
    // TODO(KoiFresh): process message
  }

  void onMessage(void Function(SignalingMessage) handler) {
    // TODO(KoiFresh): call send handler
    _onMessage = handler;
  }
}
