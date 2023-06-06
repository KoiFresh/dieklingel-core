import 'package:mqtt/models/request.dart';
import 'package:mqtt/models/response.dart';

import 'base_controller.dart';

class SnapshotController extends BaseController {
  SnapshotController(super.client) {
    client.answer("rest/snapshot/capture", _onSnapshotCapture);
  }

  Future<Response> _onSnapshotCapture(Request request) async {
    return Response.notImplemented;
  }
}
