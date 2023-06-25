import 'dart:io';

abstract class BaseRepository {
  String get homePath => Directory.current.path;
}
