import 'package:dieklingel_core/repositories/event_repository.dart';
import 'package:mqtt/mqtt.dart';

class EventService {
  final Client client;
  final EventRepository repository;

  EventService(this.client, this.repository);
}
