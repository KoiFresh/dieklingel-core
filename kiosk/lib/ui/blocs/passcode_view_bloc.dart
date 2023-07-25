import 'dart:async';
import 'dart:convert';

import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:kiosk/models/action_execute_request.dart';
import 'package:kiosk/repositories/app_repository.dart';
import 'package:logger/logger.dart';
import 'package:mqtt/mqtt_http_client.dart';

import '../states/passcode_state.dart';

class PasscodeViewBloc extends Cubit<PasscodeState> {
  final AppRepository appRepository;

  PasscodeViewBloc(this.appRepository) : super(PasscodeState());

  Future<void> submit(String passcode) async {
    MqttHttpClient client = MqttHttpClient();
    Uri uri = await appRepository.fetchMqttUri();
    String username = await appRepository.fetchMqttUsername();
    String password = await appRepository.fetchMqttPassword();
    try {
      await client.post(
        uri.resolve("actions/execute"),
        headers: {"username": username, "password": password},
        body: jsonEncode(
          ActionExecuteRequest(
            "unlock",
            environment: {"PASSCODE": passcode},
          ).toMap(),
        ),
      );
    } on TimeoutException catch (e) {
      Logger.warn(e.message ?? "timeout exception");
    }
  }
}
