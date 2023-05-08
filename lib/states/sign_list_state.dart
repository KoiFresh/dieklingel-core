import '../models/sign.dart';

class SignListState {
  final List<Sign> signs;
  final bool isLoading;

  SignListState._({
    this.signs = const [],
    this.isLoading = false,
  });

  factory SignListState.loading() {
    return SignListState._(isLoading: true);
  }

  factory SignListState.signs(List<Sign> signs) {
    return SignListState._(signs: signs);
  }
}
