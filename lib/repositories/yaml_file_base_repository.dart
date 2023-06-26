import 'dart:io';

import 'package:yaml/yaml.dart';

import 'package:path/path.dart' as path;
import '../blueprint/blueprint.dart';
import 'file_base_repository.dart';

abstract class YamlFileBaseRepository extends FileBaseRepository {
  @override
  String get configFilePath => path.join(
        Directory.current.path,
        "core.yaml",
      );

  Future<bool> validate({bool throwable = false}) async {
    Map map = await readYamlConfig();

    bool result = matchMap(
      map.cast<String, dynamic>(),
      {
        "actions": ListF.of(
          MapF.of(
            {
              "trigger": StringF,
            },
          ),
        ),
        "mqtt": MapF.of(
          {
            "uri": UriF,
            "username": StringF,
            "password": StringF,
          },
        ),
        "http": MapF.of(
          {
            "port": IntF,
          },
        ),
        "gui": MapF.of(
          {
            "timeout": IntF,
            "viewport": MapF.of(
              {
                "clip": MapF.of(
                  {
                    "top": IntF,
                    "right": IntF,
                    "bottom": IntF,
                    "left": IntF,
                  },
                )
              },
            ),
            "signs": ListF.of(
              MapF.of(
                {
                  "name": StringF,
                  "interface": FileF,
                  "audio": FileF,
                },
              ),
            )
          },
        ),
        "rtc": MapF.of(
          {
            "ice-servers": ListF.of(
              MapF.of(
                {
                  "urls": StringF,
                  "username": StringOrNull,
                  "credentials": StringOrNull
                },
              ),
            ),
          },
        )
      },
      throwable: throwable,
    );

    return result;
  }

  Future<YamlMap> readYamlConfig() async {
    String rawConfig = await readConfigFile();
    YamlMap config = loadYaml(rawConfig);

    return config;
  }
}
