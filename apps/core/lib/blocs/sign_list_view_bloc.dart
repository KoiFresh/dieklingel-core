import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:shell/shell.dart';

import '../models/action.dart';
import '../models/sign.dart';
import '../repositories/action_repository.dart';
import '../repositories/sign_repository.dart';
import '../states/sign_list_state.dart';

class SignListViewBloc extends Cubit<SignListState> {
  final ActionRepository actionRepository;
  final SignRepository signRepository;

  SignListViewBloc(
    this.signRepository,
    this.actionRepository,
  ) : super(SignListState.signs([])) {
    refresh();
  }

  Future<void> refresh() async {
    emit(SignListState.loading());
    List<Sign> signs = await signRepository.fetchAllSigns();

    emit(SignListState.signs(signs));
  }

  Future<void> ring(Sign sign) async {
    List<Action> actions = await actionRepository.fetchActionsFor("ring");

    for (Action action in actions) {
      Shell shell = Shell(environment: {
        "SIGN": sign.identifier,
      });

      await shell.run("eval", arguments: [action.lane]);
    }
  }
}
