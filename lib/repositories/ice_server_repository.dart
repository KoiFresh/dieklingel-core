import 'dart:io';

import 'package:path/path.dart' as path;
import 'package:yaml/yaml.dart';

import '../extensions/yaml_map.dart';
import '../models/ice_server.dart';

class IceServerRepository {
  final String configFilePath =
      path.normalize("/usr/share/dieklingel/core.yaml");

  Future<List<IceServer>> fetchAllIceServers() async {
    File configFile = File(configFilePath);
    String rawConfig = await configFile.readAsString();
    YamlMap config = loadYaml(rawConfig);

    List<IceServer> iceServers = config
        .get<YamlMap>("rtc")
        .get<YamlList>("ice-servers")
        .map((iceServer) => IceServer.fromYaml(iceServer))
        .toList();

    return iceServers;
  }
}
