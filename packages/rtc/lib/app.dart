import 'dart:async';
import 'dart:convert';
import 'dart:io';

import 'package:dieklingel_core_shared/flutter_shared.dart';
import 'package:dieklingel_core_shared/mqtt/mqtt_response.dart';
import 'package:flutter_webrtc/flutter_webrtc.dart';
import 'package:get_it/get_it.dart';
import 'package:rtc/signaling/signaling_message.dart';
import 'package:rtc/signaling/signaling_message_type.dart';
import 'package:rtc/utils/media_ressource.dart';
import 'package:rtc/utils/mqtt_channel.dart';
import 'package:rtc/utils/rtc_client_wrapper.dart';
import 'package:yaml/yaml.dart';

import 'config.dart';

class App {
  MqttUri? uri;
  Map<StreamSubscription, RtcClientWrapper> connections = {};

  App() {
    String? uri = Config.get<String>(kSettingsMqttUri);
    if (uri == null) {
      stderr.writeln(
        "$kSettingsMqttUri is not specified in config.yaml! The rtc package will not work like expected!",
      );
      return;
    }

    try {
      this.uri = MqttUri.fromUri(Uri.parse(uri));
    } catch (exception) {
      stderr.writeln(exception.toString());
    }
  }

  List<IceServer> iceServers() {
    return (Config.get<YamlList>(kSettingsRtcIceServersList) ?? [])
        .map((map) => IceServer.fromYaml(map))
        .toList();
  }

  Future<void> setupMqttChannels() async {
    MqttClientBloc mqtt = GetIt.I<MqttClientBloc>();
    mqtt.answer("request/rtc/connect/+", (String message) async {
      RtcClientWrapper wrapper = await RtcClientWrapper.create(
        iceServers: iceServers(),
      );
      MqttUri connUri = MqttUri.fromMap(jsonDecode(message));

      MqttChannel channel = MqttChannel(connUri.channel.toString()).remove(
        MqttChannel(uri?.channel.toString() ?? ""),
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

    mqtt.answer("request/rtc/snapshot/+", (message) async {
      MediaRessource ressource = MediaRessource();
      MediaStream? stream = await ressource.open(false, true);
      if (stream == null || stream.getVideoTracks().isEmpty) {
        return const MqttResponse(
          status: 503,
          message: "Could not open any camera!",
        );
      }

      MediaStreamTrack track = stream.getVideoTracks().first;

      // TODO: capture frame from track and send as response
      // ByteBuffer buffer = await track.captureFrame();

      ressource.close();

      return const MqttResponse(
        status: 501,
        message: "Snapshot is currently not implemented!",
      );
    });
  }

  Future<void> connect() async {
    MqttClientBloc bloc = GetIt.I<MqttClientBloc>();

    String? username = Config.get<String>(kSettingsMqttUsername);
    if (username != null) {
      bloc.usernanme.add(username);
    }

    String? password = Config.get<String>(kSettingsMqttPassword);
    if (password != null) {
      bloc.password.add(password);
    }

    bloc.uri.add(uri);
  }
}
