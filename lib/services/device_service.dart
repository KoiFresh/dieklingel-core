import 'dart:convert';

import 'package:blueprint/blueprint.dart';
import 'package:shelf/shelf.dart';
import 'package:shelf_router/shelf_router.dart';

import '../models/device.dart';
import '../repositories/device_repository.dart';

class DeviceService {
  DeviceRepository deviceRepository;

  DeviceService(this.deviceRepository);

  Handler get handler {
    final Router router = Router();

    router.get("/", (Request request) async {
      List<Device> devices;
      List<String>? signs;
      if (request.requestedUri.queryParameters.containsKey("signs")) {
        signs = request.requestedUri.queryParametersAll["signs"];
      }
      devices = await deviceRepository.fetchAllDevices(signs: signs);

      List<Map<String, dynamic>> responseBody =
          devices.map((e) => e.toMap()).toList();

      return Response.ok(
        jsonEncode(responseBody),
        headers: {
          "Content-Type": "application/json",
        },
      );
    });

    router.get("/<token>", (Request request, String token) async {
      try {
        Device device = await deviceRepository.fetchDeviceByToken(token);

        return Response.ok(
          jsonEncode(device.toMap()),
          headers: {
            "Content-Type": "application/json",
          },
        );
      } on FetchDeviceError {
        return Response.notFound("The requested device was not found");
      }
    });

    router.post("/", (Request request) async {
      Map<String, dynamic> body;
      Device device;

      try {
        body = jsonDecode(await request.readAsString());
        device = Device.fromMap(body);
      } on FormatException catch (error) {
        return Response.badRequest(body: error.toString());
      } on TypeDoesNotMatch catch (error) {
        return Response.badRequest(body: error.toString());
      }

      await deviceRepository.addDevice(device);

      return Response(
        201,
        body: jsonEncode(device.toMap()),
        headers: {
          "Content-Type": "application/json",
        },
      );
    });

    router.patch("/<token>", (Request request, String token) async {
      Map<String, dynamic> body;
      Device device;
      try {
        body = jsonDecode(await request.readAsString());
        device = Device.fromMap(body);
      } on FormatException catch (error) {
        return Response.badRequest(body: error.toString());
      } on TypeDoesNotMatch catch (error) {
        return Response.badRequest(body: error.toString());
      }

      await deviceRepository.modifyDevice(token, device);

      return Response.ok(
        jsonEncode(device.toMap()),
        headers: {
          "Content-Type": "application/json",
        },
      );
    });

    router.put("/<token>", (Request request, String token) async {
      Map<String, dynamic> body = jsonDecode(await request.readAsString());
      Device device = Device(
        signs: (body["signs"] as List).cast<String>(),
        token: token,
      );

      await deviceRepository.deleteDevice(device);
      await deviceRepository.addDevice(device);

      return Response.ok(jsonEncode(device.toMap()));
    });

    router.delete("/<token>", (Request request, String token) async {
      Device device = Device(signs: [], token: token);
      await deviceRepository.deleteDevice(device);

      return Response(204);
    });
    return router;
  }
}
