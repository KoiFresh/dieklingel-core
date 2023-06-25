import 'package:yaml/yaml.dart';

extension YamlMapExtension on YamlMap {
  T get<T>(String key) {
    if (!keys.contains(key) || value[key] is! T) {
      throw YamlException(
        "The YamlMap does not contain a key with the name '$key', available keys are: $keys",
        span,
      );
    }

    return value[key] as T;
  }

  T? read<T>(String key) {
    if (!keys.contains(key)) {
      return null;
    }
    if (value[key] is! T) {
      throw YamlException(
        "Cannot read the key with name '$key' as type '$T'",
        span,
      );
    }

    return value[key] as T;
  }
}
