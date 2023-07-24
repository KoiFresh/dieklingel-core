import 'package:blueprint/blueprint.dart';

// ignore: constant_identifier_names
const UriF = UriBluePrintField();

class UriBluePrintField extends BluePrintField {
  const UriBluePrintField();

  @override
  void match(String key, Object? value) {
    try {
      if (value is! String) {
        throw TypeDoesNotMatch(key: key, value: value, expected: String);
      }

      Uri.parse(value);
    } on FormatException catch (e) {
      throw BluePrintException(key: key, value: value, msg: e.message);
    }
  }
}
