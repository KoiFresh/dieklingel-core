import 'dart:io';

abstract class FileBaseRepository {
  String get homePath =>
      Platform.environment["SNAP_REAL_HOME"] ??
      Platform.environment["HOME"] ??
      "";

  String get configFilePath;

  Future<String> readConfigFile() async {
    File configFile = File(configFilePath);
    String rawConfig = await configFile.readAsString();

    return rawConfig;
  }
}
