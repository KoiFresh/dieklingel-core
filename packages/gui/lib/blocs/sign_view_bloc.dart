import 'dart:async';

import 'package:dieklingel_core_shared/flutter_shared.dart';
import 'package:get_it/get_it.dart';
import 'package:gui/config.dart';
import 'package:rxdart/rxdart.dart';
import 'package:yaml/yaml.dart';

import '../models/sign_options.dart';
import '../utils/mqtt_channel_constants.dart';

class SignViewBloc extends Bloc {
  final _mqttblock = GetIt.I.get<MqttClientBloc>();
  final _options = BehaviorSubject<List<SignOptions>>();
  final _click = StreamController<SignOptions>();

  Stream<List<SignOptions>> get options => _options.stream;
  Sink<SignOptions> get onClick => _click.sink;

  SignViewBloc() {
    List<SignOptions> options =
        (Config.get<YamlList>(kSettingsGuiSignsList) ?? [])
            .map((e) => SignOptions.fromYaml(e))
            .toList();

    _options.add(options);

    _click.stream.listen(
      (event) {
        _mqttblock.message.add(
          MapEntry(
            kIoActionSignClicked,
            event.identifier,
          ),
        );
      },
    );
  }

  @override
  void dispose() {
    _options.close();
    _click.close();
  }
}
