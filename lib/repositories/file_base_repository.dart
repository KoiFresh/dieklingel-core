import 'dart:io';

import 'base_repository.dart';

abstract class FileBaseRepository extends BaseRepository {
  String get configFilePath;

  Future<String> readConfigFile() async {
    File configFile = File(configFilePath);
    String rawConfig = await configFile.readAsString();

    return rawConfig;
  }
}
