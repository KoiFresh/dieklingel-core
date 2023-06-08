import 'dart:io';

abstract class BaseRepository {
  String get homePath =>
      Platform.environment["SNAP_REAL_HOME"] ??
      Platform.environment["HOME"] ??
      "";
}
