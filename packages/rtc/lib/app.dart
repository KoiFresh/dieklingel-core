import 'dart:async';
import 'dart:convert';
import 'dart:io';

import 'package:dieklingel_core_shared/flutter_shared.dart';
import 'package:dieklingel_core_shared/mqtt/mqtt_response.dart';
import 'package:get_it/get_it.dart';
import 'package:rtc/signaling/signaling_message.dart';
import 'package:rtc/signaling/signaling_message_type.dart';
import 'package:rtc/utils/mqtt_channel.dart';
import 'package:rtc/utils/rtc_client_wrapper.dart';
import 'package:yaml/yaml.dart';

import 'config.dart';

class App {
  late final MqttUri uri;
  late final String username;
  late final String password;
  late final List<IceServer> servers = [];

  App() {
    Future(() {});
  }

  Future<void> setupConfigFile() async {
    YamlMap config = await getConfig();

    uri = MqttUri.fromUri(
      Uri.parse(
        config["mqtt"]?["uri"] ?? "mqtt://127.0.0.1:1883/com.dieklingel/",
      ),
    );

    username = config["mqtt"]?["username"] ?? "";
    password = config["mqtt"]?["password"] ?? "";

    for (YamlMap ice in config["rtc"]?["ice-servers"] ?? []) {
      IceServer server;

      try {
        server = IceServer.fromYaml(ice);
      } catch (exception) {
        stdout.writeln("Skip Server: $exception");
        continue;
      }

      servers.add(server);
    }
  }

  Future<void> setupMqttChannels() async {
    MqttClientBloc mqtt = GetIt.I<MqttClientBloc>();
    mqtt.answer("request/rtc/connect/+", (String message) async {
      RtcClientWrapper wrapper = await RtcClientWrapper.create(
        iceServers: servers,
      );
      MqttUri connUri = MqttUri.fromMap(jsonDecode(message));

      MqttChannel channel = MqttChannel(connUri.channel.toString()).remove(
        MqttChannel(uri.channel.toString()),
      );

      String invite = channel.append("invite").toString();
      String answer = channel.append("answer").toString();

      StreamSubscription subscription = mqtt.watch(invite).listen(
        (event) {
          SignalingMessage message = SignalingMessage.fromJson(
            jsonDecode(event.value),
          );

          wrapper.addMessage(message);
        },
      );

      wrapper.onMessage(
        (SignalingMessage message) {
          if (message.type == SignalingMessageType.leave ||
              message.type == SignalingMessageType.error) {
            subscription.cancel();
            wrapper.dispose();

            connections.remove(subscription);
          }

          mqtt.message.add(
            ChannelMessage(
              answer,
              jsonEncode(message.toJson()),
            ),
          );
        },
      );

      await wrapper.ressource.open(true, true);

      connections[subscription] = wrapper;
      return MqttResponse.ok;
    });
  }

  Future<void> connect() async {
    MqttClientBloc mqtt = GetIt.I<MqttClientBloc>();
    mqtt.uri.add(uri);
  }

  Map<StreamSubscription, RtcClientWrapper> connections = {};
}
