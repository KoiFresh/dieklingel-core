import 'package:mqtt/models/request.dart';
import 'package:mqtt/mqtt.dart';

import '../repositories/action_repository.dart';
import '../repositories/ice_server_repository.dart';

class RestController {
  final Client client;
  final IceServerRepository iceServerRepository;
  final ActionRepository actionRepository;

  RestController(this.client, this.iceServerRepository, this.actionRepository) {
    client.answer("request/rtc/connect", _onRtcConnectionRequest);
    client.answer("rest/snapshot", _onSnapshotRequest);
    client.answer("rest/action/trigger", _onActionTrigger);
  }

  Future<Response> _onSnapshotRequest(Request request) async {
    return Response.notImplemented;
  }

  Future<Response> _onRtcConnectionRequest(Request request) async {
    return Response.notImplemented;
  }

  Future<Response> _onActionTrigger(Request request) async {
    return Response.notImplemented;
  }
}
