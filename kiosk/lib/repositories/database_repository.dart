import 'package:hive/hive.dart';

import 'base_repository.dart';
import 'package:path/path.dart' as path;

class DatabaseRepository extends BaseRepository {
  DatabaseRepository() {
    Hive.init(path.join(homePath, "storage"));
  }
}
