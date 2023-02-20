import 'dart:async';

import 'package:dieklingel_core_shared/shared.dart';
import 'package:get_it/get_it.dart';
import 'package:rxdart/rxdart.dart';

import '../models/sign_options.dart';
import '../utils/mqtt_channel_constants.dart';

class SignViewBloc extends Bloc {
  final _mqttblock = GetIt.I.get<MqttClientBloc>();
  final _options = BehaviorSubject<List<SignOptions>>();
  final _click = StreamController<SignOptions>();

  Stream<List<SignOptions>> get options => _options.stream;
  Sink<SignOptions> get onClick => _click.sink;

  SignViewBloc() {
    _options.add(SignOptions.boxx.values.toList());
    SignOptions.boxx.watch().listen((event) {
      _options.add(SignOptions.boxx.values.toList());
    });

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
