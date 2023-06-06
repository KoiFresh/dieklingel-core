import '../models/event.dart';
import 'yaml_file_base_repository.dart';

class EventRepository extends YamlFileBaseRepository {
  Future<List<Event>> fetchAllEvents({
    DateTime? after,
    DateTime? until,
  }) {
    if (after != null || until != null) {
      throw UnimplementedError("Sorting by timestamps is not supported yet");
    }

    throw UnimplementedError();
  }

  Future<Event?> fetchEventByHash(String hash) {
    throw UnimplementedError();
  }
}
