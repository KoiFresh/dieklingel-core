import 'dart:io';

import 'package:yaml/yaml.dart';
import 'package:path/path.dart' as p;

const String kSettingsMqttUri = "mqtt.uri";
const String kSettingsMqttUsername = "mqtt.username";
const String kSettingsMqttPassword = "mqtt.password";

const String kSettingsGuiViewportClipTop = "gui.viewport.clip.top";
const String kSettingsGuiViewportClipRight = "gui.viewport.clip.right";
const String kSettingsGuiViewportClipBottom = "gui.viewport.clip.bottom";
const String kSettingsGuiViewportClipLeft = "gui.viewport.clip.left";

const String kSettingsGuiSignsList = "gui.signs";

const String kSettingsGuiScreensaverTimeout = "gui.screensaver.timeout";
const String kSettingsGuiScreensaverEnabled = "gui.screensaver.enabled";
const String kSettingsGuiScreensaverFile = "gui.screensaver.file";

Future<YamlMap> getConfig() async {
  YamlMap result = YamlMap();
  String path = p.join(
    Platform.environment["SNAP_REAL_HOME"] ??
        Platform.environment["HOME"] ??
        "",
    "dieklingel",
    "config.yaml",
  );
  final configFile = File(path);
  try {
    result = await loadYaml(
      await configFile.readAsString(),
    );
  } catch (exception) {
    stderr.writeln(exception);
  }
  return result;
}

class Config {
  static YamlMap _config = YamlMap();

  static Future<void> read() async {
    String path = p.join(
      Platform.environment["SNAP_REAL_HOME"] ??
          Platform.environment["HOME"] ??
          "",
      "dieklingel",
      "config.yaml",
    );
    final configFile = File(path);
    try {
      _config = await loadYaml(
        await configFile.readAsString(),
      );
    } catch (exception) {
      stderr.writeln(exception);
    }
  }

  static T? get<T>(String property) {
    assert(
      dynamic is! T,
      "Always specify the return type your like to request, like get<String>('some.path');",
    );
    List<String> sections = property.split(".");
    YamlMap map = _config;
    for (String section in sections) {
      if (section == sections.last && map[section] is T) {
        return map[section];
      }
      if (map[section] is! YamlMap) {
        return null;
      }
      map = map[section];
    }
    return null;
  }
}
