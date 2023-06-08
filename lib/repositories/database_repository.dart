import 'package:sqflite_common_ffi/sqflite_ffi.dart';

import 'base_repository.dart';
import 'package:path/path.dart' as path;

class DatabaseRepository extends BaseRepository {
  DatabaseRepository() {
    databaseFactory = databaseFactoryFfi;
  }

  Future<Database> open() {
    return openDatabase(
      path.join(homePath, "dieklingel", "dieklingel.sqlite"),
      version: 2,
      onCreate: (db, version) {
        return db.execute("""
          CREATE TABLE devices
          (
            token TEXT NOT NULL PRIMARY KEY
          );

          CREATE TABLE signs
          (
            name TEXT NOT NULL,
            device_token TEXT NOT NULL,
            FOREIGN KEY (device_token) REFERENCES devices(token),
            PRIMARY KEY (name, device_token)
          );
        """);
      },
    );
  }
}
