import 'package:dieklingel_core/utils/media_ressource.dart';
import 'package:flutter_webrtc/flutter_webrtc.dart';
import 'package:sip_ua/sip_ua.dart';

import 'handler.dart';

class RtcHandler extends Handler implements SipUaHelperListener {
  final MediaRessource _ressource = MediaRessource();

  final _sipua = SIPUAHelper();

  RtcHandler() {
    _sipua.addSipUaHelperListener(this);
  }

  @override
  void run() {
    /* UaSettings settings = UaSettings()
      ..webSocketUrl = "ws://192.168.64.1:8088/ws"
      ..uri = "400@192.168.64.1"
      ..password = "400"
      ..authorizationUser = "400"
      ..displayName = "Core"
      ..dtmfMode = DtmfMode.RFC2833
      ..userAgent = "Dart SIP Client v1.0.0"
      ..iceServers = [
        {
          "urls": "stun:stun.l.google.com:19302",
          "username": "",
          "credential": "",
        },
        {
          "urls": "stun:a.relay.metered.ca:80",
        },
        {
          "urls": "turn:a.relay.metered.ca:80",
          "username": "b2d44a5253ab2ddd58522b34",
          "credential": "zeFyJa3y04YHpYs6",
        },
        {
          "urls": "turn:a.relay.metered.ca:80?transport=tcp",
          "username": "b2d44a5253ab2ddd58522b34",
          "credential": "zeFyJa3y04YHpYs6",
        },
        {
          "urls": "turn:a.relay.metered.ca:443",
          "username": "b2d44a5253ab2ddd58522b34",
          "credential": "zeFyJa3y04YHpYs6",
        },
        {
          "urls": "turn:a.relay.metered.ca:443?transport=tcp",
          "username": "b2d44a5253ab2ddd58522b34",
          "credential": "zeFyJa3y04YHpYs6",
        },
      ];*/
    UaSettings settings = UaSettings()
      ..webSocketUrl = "ws://85.214.41.43:8088/ws"
      ..uri = "400@85.214.41.43"
      ..password = "400"
      ..authorizationUser = "400"
      ..displayName = "Core kj"
      ..dtmfMode = DtmfMode.RFC2833
      ..userAgent = "Dart SIP Client v1.0.0"
      ..iceServers = [
        {
          "url": "stun:stun.l.google.com:19302",
          "username": "",
          "credential": "",
        },
        {
          "url": "stun:a.relay.metered.ca:80",
        },
        {
          "url": "turn:a.relay.metered.ca:80",
          "username": "b2d44a5253ab2ddd58522b34",
          "credential": "zeFyJa3y04YHpYs6",
        },
        {
          "url": "turn:a.relay.metered.ca:80?transport=tcp",
          "username": "b2d44a5253ab2ddd58522b34",
          "credential": "zeFyJa3y04YHpYs6",
        },
        {
          "url": "turn:a.relay.metered.ca:443",
          "username": "b2d44a5253ab2ddd58522b34",
          "credential": "zeFyJa3y04YHpYs6",
        },
        {
          "url": "turn:a.relay.metered.ca:443?transport=tcp",
          "username": "b2d44a5253ab2ddd58522b34",
          "credential": "zeFyJa3y04YHpYs6",
        },
      ];

    _sipua.start(settings);
    //_sipua.register();
  }

  @override
  void callStateChanged(Call call, CallState state) async {
    print(state.state);
    switch (state.state) {
      case CallStateEnum.NONE:
        // TODO: Handle this case.
        break;
      case CallStateEnum.STREAM:
        // TODO: Handle this case.
        break;
      case CallStateEnum.UNMUTED:
        // TODO: Handle this case.
        break;
      case CallStateEnum.MUTED:
        // TODO: Handle this case.
        break;
      case CallStateEnum.CONNECTING:
        // TODO: Handle this case.
        break;
      case CallStateEnum.PROGRESS:
        print(call.direction);
        if (call.direction == "INCOMING") {
          MediaStream? stream = await _ressource.open(true, true);
          if (stream == null) {
            print("start without stream");
          }
          call.answer({}, mediaStream: stream);
        }
        break;
      case CallStateEnum.FAILED:
      case CallStateEnum.ENDED:
        call.hangup();
        _ressource.close();
        break;
      case CallStateEnum.ACCEPTED:
        // TODO: Handle this case.
        break;
      case CallStateEnum.CONFIRMED:
        // TODO: Handle this case.
        break;
      case CallStateEnum.REFER:
        // TODO: Handle this case.
        break;
      case CallStateEnum.HOLD:
        // TODO: Handle this case.
        break;
      case CallStateEnum.UNHOLD:
        // TODO: Handle this case.
        break;
      case CallStateEnum.CALL_INITIATION:
        // TODO: Handle this case.
        break;
    }

    // TODO: implement callStateChanged
  }

  @override
  void onNewMessage(SIPMessageRequest msg) {
    print(msg);
    // TODO: implement onNewMessage
  }

  @override
  void onNewNotify(Notify ntf) {
    print(ntf);
    // TODO: implement onNewNotify
  }

  @override
  void registrationStateChanged(RegistrationState state) {
    print(state);
  }

  @override
  void transportStateChanged(TransportState state) {
    print(state);
    // TODO: implement transportStateChanged
  }
}
