import 'package:flutter/material.dart';
import 'package:yaml/yaml.dart';

import '../extensions/yaml_map.dart';
import 'file_base_repository.dart';

class AppRepository extends YamlFileBaseRepository {
  EdgeInsets? _viewportClip;
  Uri? _mqttUri;
  String? _mqttUsername;
  String? _mqttPassword;
  Duration? _screenTimeout;

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

  Future<Uri> fetchMqttUri() {
    throw UnimplementedError();
  }

  Future<String> fetchMqttUsername() {
    throw UnimplementedError();
  }

  Future<String> fetchMqttPassword() {
    throw UnimplementedError();
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
