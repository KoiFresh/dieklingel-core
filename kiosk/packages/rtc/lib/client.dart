import 'package:flutter/foundation.dart';
import 'package:flutter_webrtc/flutter_webrtc.dart';
import 'package:rtc/media_ressource.dart';
import 'package:rtc/utils/rtc_transceiver.dart';
import 'package:rtc/utils/speaker_state.dart';

import 'package:rtc/utils/microphone_state.dart';

import 'client_interface.dart';
import 'models/ice_server.dart';

class Client implements ClientInterface {
  final MediaRessource ressource = MediaRessource();
  RTCVideoRenderer? _renderer;
  RTCPeerConnection? _connection;
  MicrophoneState _microphoneState = MicrophoneState.muted;
  SpeakerState _speakerState = SpeakerState.muted;

  RTCVideoRenderer? get renderer => _renderer;

  @override
  MicrophoneState get microphoneState => _microphoneState;

  @override
  set microphoneState(MicrophoneState state) {
    _microphoneState = state;
    _applyMicrophoneSettings();
  }

  @override
  SpeakerState get speakerState => _speakerState;

  @override
  set speakerState(SpeakerState state) {
    _speakerState = state;
    _applySpeakerSettings();
  }

  @override
  void Function(RTCSessionDescription answer)? onAnswer;

  @override
  void Function()? onClose;

  @override
  void Function(RTCIceCandidate candidate)? onIceCandidate;

  @override
  void Function(RTCSessionDescription offer)? onOffer;

  @override
  Future<void> addAnswer(RTCSessionDescription answer) async {
    if (_connection == null) {
      throw Exception("cannot add answer before the connection is open!");
    }

    await _connection!.setRemoteDescription(answer);
  }

  @override
  Future<void> addIceCandidate(RTCIceCandidate candidate) async {
    if (_connection == null) {
      throw Exception("cannot add candidate before the connection is open!");
    }

    await _connection!.addCandidate(candidate);
  }

  @override
  Future<void> addOffer(RTCSessionDescription offer) async {
    if (_connection == null) {
      throw Exception("cannot add offer before the connection is open!");
    }

    MediaStream? stream = ressource.stream;
    if (null != stream) {
      for (MediaStreamTrack track in stream.getTracks()) {
        _connection!.addTrack(track, stream);
      }
    }

    await _connection!.setRemoteDescription(offer);
    final local = await _connection!.createAnswer();
    await _connection!.setLocalDescription(local);

    onAnswer?.call(local);
  }

  @override
  Future<void> close() async {
    ressource.close();
    await _connection?.close();
    if (_connection == null) {}
    onClose?.call();
    _connection = null;
    _renderer = null;
  }

  @override
  RTCPeerConnectionState get connectionState {
    return _connection?.connectionState ??
        RTCPeerConnectionState.RTCPeerConnectionStateClosed;
  }

  @override
  Future<void> open({
    bool createOffer = true,
    List<RtcTransceiver> transceiver = const [],
    List<IceServer> servers = const [],
  }) async {
    if (_connection != null || _renderer != null) {
      throw Exception(
        "The connection is already in open state, close it before open again!",
      );
    }

    _renderer = RTCVideoRenderer();
    await _renderer?.initialize();

    List<Map<String, dynamic>> iceServers = servers
        .map(
          (e) => {
            "urls": e.urls,
            "username": e.username,
            "credential": e.credential
          },
        )
        .toList();

    Map<String, dynamic> configuration = {
      "iceServers": iceServers,
      "sdpSemantics": "unified-plan",
    };

    _connection = await createPeerConnection(configuration);
    _connection!
      ..onIceCandidate = _onIceCandidate
      ..onConnectionState = _onConnectionState
      ..onTrack = _onTrack;

    if (!createOffer) {
      return;
    }

    RTCSessionDescription offer = await _connection!.createOffer();
    await _connection!.setLocalDescription(offer);

    MediaStream? stream = ressource.stream;
    if (null != stream) {
      for (MediaStreamTrack track in stream.getTracks()) {
        _connection!.addTrack(track, stream);
      }
    }

    onOffer?.call(offer);
    _applyMicrophoneSettings();
    _applySpeakerSettings();
  }

  void _onIceCandidate(RTCIceCandidate candidate) {
    onIceCandidate?.call(candidate);
  }

  void _onConnectionState(RTCPeerConnectionState state) {
    switch (state) {
      case RTCPeerConnectionState.RTCPeerConnectionStateFailed:
      case RTCPeerConnectionState.RTCPeerConnectionStateDisconnected:
        close();
        break;
      default:
        break;
    }
  }

  void _onTrack(RTCTrackEvent event) {
    if (renderer == null) {
      return;
    }

    renderer!.srcObject = event.streams.first;
    _applyMicrophoneSettings();
    _applySpeakerSettings();
  }

  void _applyMicrophoneSettings() {
    switch (_microphoneState) {
      case MicrophoneState.muted:
        ressource.stream?.getAudioTracks().forEach((track) {
          Helper.setMicrophoneMute(true, track);
        });
        break;
      case MicrophoneState.unmuted:
        ressource.stream?.getAudioTracks().forEach((track) {
          Helper.setMicrophoneMute(false, track);
        });
        break;
    }
  }

  void _applySpeakerSettings() {
    switch (_speakerState) {
      case SpeakerState.muted:
        renderer?.srcObject?.getAudioTracks().forEach((track) {
          track.enabled = false;
        });
        break;
      case SpeakerState.headphone:
        renderer?.srcObject?.getAudioTracks().forEach((track) {
          track.enabled = true;
          if (!kIsWeb) {
            track.enableSpeakerphone(true);
          }
        });
        break;
      case SpeakerState.speaker:
        renderer?.srcObject?.getAudioTracks().forEach((track) {
          track.enabled = true;
          if (!kIsWeb) {
            track.enableSpeakerphone(false);
          }
        });
        break;
    }
  }
}
