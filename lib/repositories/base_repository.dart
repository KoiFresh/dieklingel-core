import 'dart:io';

import 'package:path/path.dart';

abstract class BaseRepository {
  String get homePath =>
      Platform.environment["SNAP_USER_COMMON"] ??
      join(Platform.environment["HOME"] ?? "/etc", "dieklingel");
}
