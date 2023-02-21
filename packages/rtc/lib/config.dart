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
