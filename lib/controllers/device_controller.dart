import 'package:mqtt/models/request.dart';
import 'package:mqtt/models/response.dart';

import '../repositories/device_repository.dart';
import 'base_controller.dart';

class DeviceController extends BaseController {
  final DeviceRepository deviceRepository;

  DeviceController(super.client, this.deviceRepository) {
    client.answer("rest/devices/list", _onDevicesList);
    client.answer("rest/devices/add", _onDevicesAdd);
    client.answer("rest/devices/remove", _onDevicesRemove);
  }

  Future<Response> _onDevicesList(Request request) async {
    return Response.notImplemented;
  }

  Future<Response> _onDevicesAdd(Request request) async {
    return Response.notImplemented;
  }

  Future<Response> _onDevicesRemove(Request request) async {
    return Response.notImplemented;
  }
}
