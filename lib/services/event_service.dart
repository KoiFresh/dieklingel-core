import 'dart:io';

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

    List<Future<List<ProcessResult>>> processes = [];

    for (Event action in filterd) {
      Shell shell = Shell(environment: environment);
      Future<List<ProcessResult>> result = shell.run(action.command);
      processes.add(result);
    }

    await Future.wait(processes);
  }
}
