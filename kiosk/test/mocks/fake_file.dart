import 'dart:io';

import 'package:flutter_test/flutter_test.dart';

class FakeFile extends Fake implements File {
  @override
  final String path;

  final bool fileExists;

  FakeFile(this.path, {this.fileExists = false});

  @override
  bool existsSync() {
    return fileExists;
  }

  @override
  bool operator ==(Object other) {
    if (other is! File) {
      return false;
    }

    return path == other.path && existsSync() == other.existsSync();
  }

  @override
  int get hashCode => Object.hash(path, fileExists);
}
