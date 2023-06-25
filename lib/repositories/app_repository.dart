import 'package:flutter/material.dart';
import 'package:yaml/yaml.dart';

import '../extensions/yaml_map.dart';
import 'yaml_file_base_repository.dart';

class AppRepository extends YamlFileBaseRepository {
  EdgeInsets? _viewportClip;
  Uri? _mqttUri;
  String? _mqttUsername;
  String? _mqttPassword;
  Duration? _screenTimeout;
  int? _httpPort;

  Future<EdgeInsets> fetchViewportClip({bool useCache = true}) async {
    EdgeInsets? clip = _viewportClip;

    if (clip != null && useCache) {
      return clip;
    }

    YamlMap config = await readYamlConfig();
    YamlMap viewportClip = config
        .get<YamlMap>("gui")
        .get<YamlMap>("viewport")
        .get<YamlMap>("clip");

    clip = EdgeInsets.fromLTRB(
      viewportClip.get<int>("left").toDouble(),
      viewportClip.get<int>("top").toDouble(),
      viewportClip.get<int>("right").toDouble(),
      viewportClip.get<int>("bottom").toDouble(),
    );

    _viewportClip = clip;
    return clip;
  }

  Future<Uri> fetchMqttUri({bool useCache = true}) async {
    Uri? uri = _mqttUri;

    if (uri != null && useCache) {
      return uri;
    }

    YamlMap config = await readYamlConfig();
    String rawUri = config.get<YamlMap>("mqtt").get<String>("uri");
    uri = Uri.parse(rawUri);

    _mqttUri = uri;
    return uri;
  }

  Future<String> fetchMqttUsername({bool useCache = true}) async {
    String? username = _mqttUsername;

    if (username != null && useCache) {
      return username;
    }

    YamlMap config = await readYamlConfig();
    username = config.get<YamlMap>("mqtt").get<String>("username");

    _mqttUsername = username;
    return username;
  }

  Future<String> fetchMqttPassword({bool useCache = true}) async {
    String? password = _mqttPassword;

    if (password != null && useCache) {
      return password;
    }

    YamlMap config = await readYamlConfig();
    password = config.get<YamlMap>("mqtt").get<String>("password");

    _mqttPassword = password;
    return password;
  }

  Future<int> fetchHttpPort({bool useCache = true}) async {
    int? port = _httpPort;

    if (port != null && useCache) {
      return port;
    }

    YamlMap config = await readYamlConfig();
    port = config.read<YamlMap>("http")?.read<int>("port") ?? 80;

    _httpPort = port;
    return port;
  }

  Future<Duration> fetchScreenTimeout({bool useCache = true}) async {
    Duration? timeout = _screenTimeout;

    if (timeout != null && useCache) {
      return timeout;
    }

    YamlMap config = await readYamlConfig();
    int value = config.get<YamlMap>("gui").get<int>("timeout");

    timeout = Duration(seconds: value);
    _screenTimeout = timeout;

    return timeout;
  }
}
