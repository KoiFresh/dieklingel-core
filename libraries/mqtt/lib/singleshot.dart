import 'package:uuid/uuid.dart';

import 'client.dart';
import 'models/mqtt_uri.dart';
import 'models/request.dart';
import 'models/response.dart';

abstract class Singleshot {
  static Future<Response> fetch(
    MqttUri uri, {
    String? username,
    String? password,
    required Request request,
  }) async {
    String uuid = const Uuid().v4();

    Client client = Client();
    await client.connect(
      uri,
      username: username,
      password: password,
      identifier: uuid,
    );

    Response response = await client.request(request);
    await client.disconnect();

    return response;
  }

  static Future<Response> get(
    MqttUri uri, {
    String? username,
    String? password,
    dynamic body,
  }) async {
    String uuid = const Uuid().v4();

    Client client = Client();
    await client.connect(
      uri,
      username: username,
      password: password,
      identifier: uuid,
    );

    Request request = Request.get(
      request: Uri(path: "/"),
      response: Uri(path: uuid),
      body: body,
    );
    Response response = await client.request(request);
    await client.disconnect();

    return response;
  }

  static Future<Response> post(
    MqttUri uri, {
    String? username,
    String? password,
    dynamic body,
    Map<String, String> headers = const {},
  }) async {
    String uuid = const Uuid().v4();

    Client client = Client();
    await client.connect(
      uri,
      username: username,
      password: password,
      identifier: uuid,
    );

    Request request = Request.post(
      request: Uri(path: uri.channel),
      response: Uri(path: uuid),
      body: body,
    );
    Response response = await client.request(request);
    await client.disconnect();

    return response;
  }
}
