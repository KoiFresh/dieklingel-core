import 'dart:io';

import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:shell/shell.dart';

import '../models/action.dart';
import '../repositories/action_repository.dart';
import '../states/passcode_state.dart';

class PasscodeViewBloc extends Cubit<PasscodeState> {
  final ActionRepository actionRepository;

  PasscodeViewBloc(this.actionRepository) : super(PasscodeState());

  Future<void> submit(String passcode) async {
    List<Action> actions = await actionRepository.fetchActionsFor("unlock");

    for (Action action in actions) {
      Shell shell = Shell(environment: {
        "PASSCODE": passcode,
      });

      await shell.run("eval", arguments: [action.lane]);
    }
  }
}
