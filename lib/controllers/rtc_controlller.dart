import 'package:mqtt/models/request.dart';
import 'package:mqtt/models/response.dart';

import 'base_controller.dart';

class RtcController extends BaseController {
  RtcController(super.client) {
    client.answer("rest/rtc/connect", _onRtcConnect);
  }

  Future<Response> _onRtcConnect(Request request) async {
    return Response.notImplemented;
  }
}
