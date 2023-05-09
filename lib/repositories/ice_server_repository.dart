import 'dart:io';

import 'package:path/path.dart' as path;
import 'package:yaml/yaml.dart';

import '../extensions/yaml_map.dart';
import '../models/ice_server.dart';
import 'file_base_repository.dart';

class IceServerRepository extends YamlFileBaseRepository {
  Future<List<IceServer>> fetchAllIceServers() async {
    YamlMap config = await readYamlConfig();

    List<IceServer> iceServers = config
        .get<YamlMap>("rtc")
        .get<YamlList>("ice-servers")
        .map((iceServer) => IceServer.fromYaml(iceServer))
        .toList();

    return iceServers;
  }
}
