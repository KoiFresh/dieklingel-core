import 'package:mqtt/models/request.dart';
import 'package:mqtt/models/response.dart';

import '../repositories/device_repository.dart';
import 'base_controller.dart';

class DeviceController extends BaseController {
  final DeviceRepository deviceRepository;

  DeviceController(super.client, this.deviceRepository) {
    client.answer("rest/devices", _onDevices);
  }

  Future<Response> _onDevices(Request request) async {
    switch (request.method.toUpperCase()) {
      case "POST":
        return _onDevicesPost(request);
    }

    return Response.notImplemented;
  }

  Future<Response> _onDevicesPost(Request request) async {
    return Response.notImplemented;
  }
}
