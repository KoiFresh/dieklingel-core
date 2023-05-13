import 'package:args/command_runner.dart';
import 'package:dieklingel_core/apps/snapshot_command.dart';

void main(List<String> args) {
  CommandRunner runner = CommandRunner(
    "dieklingel.exec",
    "A lightweight dieKlingel client. This client could be used to interact with a running instance of the dieKlingel core.",
  )..addCommand(SnapshotCommand());

  runner.run(args);
}
