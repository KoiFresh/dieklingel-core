import 'dart:async';
import 'dart:convert';
import 'dart:io';

import 'package:dieklingel_core_shared/mqtt/mqtt_response.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'package:rxdart/rxdart.dart';
import 'package:uuid/uuid.dart';

import '../bloc/bloc.dart';
import '../mqtt/mqtt_client_state.dart';

import '../mqtt/mqtt_server_client_factory.dart'
    if (dart.library.js) '../mqtt/mqtt_browser_client_factory.dart';
import '../models/mqtt_uri.dart';

typedef ChannelMessage = MapEntry<String, String>;
typedef FilterFunction = String? Function(String);
typedef HandlerFunction = Future<MqttResponse> Function(String);

class MqttClientBloc extends Bloc {
  final Map<String, BehaviorSubject<ChannelMessage>> _subscribtions = {};
  final Map<String, FilterFunction> _filters = {};

  final _uri = BehaviorSubject<MqttUri?>();
  final _username = BehaviorSubject<String>.seeded("");
  final _password = BehaviorSubject<String>.seeded("");
  final _state = BehaviorSubject<MqttClientState>();
  final _message = StreamController<MapEntry<String, String>>.broadcast();

  Sink<MqttUri?> get uri => _uri.sink;
  Stream<MqttClientState> get state => _state.stream;
  StreamSink<MapEntry<String, String>> get message => _message.sink;
  StreamSink<String> get usernanme => _username.sink;
  StreamSink<String> get password => _password.sink;

  MqttClient? _client;

  MqttClientBloc() {
    _uri.stream.listen((event) {
      _connect();
    });

    /* _username.stream.listen((event) {
      _connect();
    });*/

    /* _password.stream.listen((event) {
      _connect();
    });*/

    _message.stream.listen((event) {
      _publish(event.key, event.value);
    });
  }

  Future<void> _connect() async {
    _client?.disconnect();

    MqttUri uri;
    if (!_uri.hasValue || _uri.value == null) {
      return;
    }
    uri = _uri.value!;

    String scheme = uri.websocket
        ? uri.ssl
            ? "wss://"
            : "ws://"
        : "";

    _client = MqttClientFactory.create("$scheme${uri.host}", const Uuid().v4())
      ..port = uri.port
      ..keepAlivePeriod = 20
      ..setProtocolV311()
      ..autoReconnect = true
      ..onConnected = () {
        _state.add(MqttClientState.connected);
      }
      ..onDisconnected = () {
        _state.add(MqttClientState.disconnected);
      };

    try {
      await _client?.connect(_username.value, _password.value);
    } on SocketException {
      rethrow;
    }

    _client?.updates!.listen((event) {
      MqttPublishMessage rec = event[0].payload as MqttPublishMessage;
      final String topic = event[0].topic;
      List<int> messageAsBytes = rec.payload.message;
      String message = utf8.decode(messageAsBytes);

      for (MapEntry<String, FilterFunction> entry in _filters.entries) {
        List<String> channels =
            "${_uri.value!.channel}/${entry.key}".split("/");
        channels.removeWhere((element) => element.isEmpty);
        String channel = channels.join("/");

        RegExp regExp = RegExp(
          channel.replaceAll("\\+", "[^/]+").replaceAll("#", ".+"),
        );

        if (!regExp.hasMatch(topic)) {
          continue;
        }

        String? modified = entry.value(message);
        if (modified == null) {
          return;
        }
        message = modified;
      }

      // TODO(KoiFresh): use for loop
      _subscribtions.forEach((key, value) {
        List<String> channels = "${_uri.value!.channel}/$key".split("/");
        channels.removeWhere((element) => element.isEmpty);
        String channel = channels.join("/");

        RegExp regExp = RegExp(
          "^${channel.replaceAll("/+", "/[^/]+").replaceAll("#", ".+")}\$",
        );

        if (regExp.hasMatch(topic)) {
          value.add(
            ChannelMessage(
              topic.replaceFirst(_uri.value?.channel ?? "", ""),
              message,
            ),
          );
        }
      });
    });

    _client?.subscribe("${uri.channel}#", MqttQos.exactlyOnce);
  }

  void filter(String channel, String? Function(String) filter) {
    _filters[channel] = filter;
  }

  Stream<ChannelMessage> watch(String channel) {
    StreamController<ChannelMessage> controller = _subscribtions.putIfAbsent(
      channel,
      () => BehaviorSubject(),
    );

    controller.onCancel = () {
      _subscribtions.remove(channel);
    };

    return controller.stream;
  }

  void _publish(String topic, String message) {
    if (_state.valueOrNull != MqttClientState.connected) {
      throw "the mclient has to be connected, before publish";
    }

    List<String> topics = "${_uri.value?.channel}$topic".split("/");
    topics.removeWhere((element) => element.isEmpty);
    topic = topics.join("/");

    MqttClientPayloadBuilder builder = MqttClientPayloadBuilder();
    builder.addUTF8String(message);

    _client!.publishMessage(
      topic,
      MqttQos.exactlyOnce,
      builder.payload!,
    );
  }

  Future<void> disconnect() async {
    _client?.disconnect();
    _client = null;
  }

  @override
  void dispose() {
    _uri.close();
    _username.close();
    _password.close();
    _state.close();
    _message.close();
    _subscribtions.forEach((key, value) {
      value.close();
    });
  }
}

extension Handle on MqttClientBloc {
  StreamSubscription<ChannelMessage> answer(
    String channel,
    HandlerFunction handler,
  ) {
    return watch(channel).listen(
      (event) async {
        MqttResponse response = await handler(event.value);
        message.add(
          ChannelMessage(
            "${event.key}/response",
            jsonEncode(
              response.toMap(),
            ),
          ),
        );
      },
    );
  }

  Future<MqttResponse> request(
    String channel,
    String message, {
    Duration timeout = const Duration(seconds: 30),
  }) async {
    Completer<String?> completer = Completer<String?>();
    StreamSubscription subscription = watch("$channel/response").listen(
      (event) {
        completer.complete(event.value);
      },
    );

    this.message.add(ChannelMessage(channel, message));
    String? result = await completer.future.timeout(
      timeout,
      onTimeout: () => null,
    );

    await subscription.cancel();

    MqttResponse response;

    if (result == null) {
      return const MqttResponse(status: -1, message: "timeout");
    }

    try {
      response = MqttResponse.fromMap(jsonDecode(result));
    } catch (exception) {
      return MqttResponse(status: -1, message: exception.toString());
    }

    return response;
  }
}
