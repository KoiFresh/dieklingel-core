import 'package:yaml/yaml.dart';

extension BetterYamlMap on YamlMap {
  T get<T>(String key) {
    if (!keys.contains(key) || value[key] is! T) {
      throw YamlException(
        "The YamlMap does not contain a key with the name '$key', available keys are: $keys",
        span,
      );
    }

    return value[key] as T;
  }
}
