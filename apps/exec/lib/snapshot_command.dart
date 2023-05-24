import 'dart:io';

import 'package:args/command_runner.dart';
import 'package:mqtt/mqtt.dart';
import 'package:uuid/uuid.dart';

import '../repositories/app_repository.dart';

class SnapshotCommand extends Command {
  final AppRepository appRepository = AppRepository();

  @override
  String get description =>
      "Take a Snapshot, from the core use the config file";

  @override
  String get name => "snapshot";

  @override
  Future<void> run() async {
    MqttUri uri = MqttUri.fromUri(await appRepository.fetchMqttUri());
    String username = await appRepository.fetchMqttUsername();
    String password = await appRepository.fetchMqttPassword();

    Request request = Request(
      request: Uri(path: "rest/snapshot"),
      response: Uri(path: const Uuid().v4()),
    );

    Response response = await Singleshot.fetch(
      uri,
      username: username,
      password: password,
      request: request,
    );

    if (response.status != 200) {
      stderr.writeln("Could not fetch a Snpashot: ${response.message}");
      return;
    }

    stdout.writeln(response.body);
  }
}
