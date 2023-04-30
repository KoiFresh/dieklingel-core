import 'package:dieklingel_core/models/trigger.dart';
import 'package:process_run/shell.dart';

import '../models/event.dart';

class EventService {
  final List<Event> events;

  EventService({
    this.events = const [],
  });

  void trigger(
    Trigger trigger, {
    Map<String, String> environment = const {},
  }) async {
    final filterd = events.where((e) => e.trigger == trigger);

    for (Event action in filterd) {
      Shell shell = Shell(environment: environment);
      shell.run(action.command);
    }
  }
}
