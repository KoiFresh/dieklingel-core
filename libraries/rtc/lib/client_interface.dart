import 'package:flutter_webrtc/flutter_webrtc.dart';
import 'package:rtc/utils/microphone_state.dart';
import 'package:rtc/utils/speaker_state.dart';

abstract class ClientInterface {
  void Function(RTCIceCandidate)? onIceCandidate;
  void Function(RTCSessionDescription)? onOffer;
  void Function(RTCSessionDescription)? onAnswer;
  void Function()? onClose;

  RTCPeerConnectionState get connectionState;

  SpeakerState get speakerState;

  set speakerState(SpeakerState state);

  MicrophoneState get microphoneState;

  set microphoneState(MicrophoneState state);

  Future<void> addOffer(RTCSessionDescription offer);

  Future<void> addAnswer(RTCSessionDescription answer);

  Future<void> addIceCandidate(RTCIceCandidate candidate);

  Future<void> open();

  Future<void> close();
}
