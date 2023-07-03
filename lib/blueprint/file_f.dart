import 'dart:io';

import 'package:blueprint/blueprint.dart';

// ignore: constant_identifier_names
const FileF = FileBluePrintField();

class FileBluePrintField extends BluePrintField {
  final bool existForSuccess;

  const FileBluePrintField({this.existForSuccess = false});

  FileBluePrintField exist() {
    return const FileBluePrintField(existForSuccess: true);
  }

  @override
  void match(String key, Object? value) {
    try {
      if (value is! String) {
        throw TypeDoesNotMatch(key: key, value: value, expected: String);
      }

      File file = File(value);
      bool exists = file.existsSync();

      if (existForSuccess && !exists) {
        throw BluePrintException(
          key: key,
          value: value,
          msg: "The referenced file '$key':'$value' does not exist.",
        );
      }
    } on FormatException catch (e) {
      throw BluePrintException(key: key, value: value, msg: e.message);
    }
  }
}
