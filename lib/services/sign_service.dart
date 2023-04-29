import 'package:dieklingel_core/repositories/sign_repository.dart';

import '../models/sign.dart';

class SignService {
  final SignRepository repository;

  SignService(this.repository);

  Future<List<Sign>> get signs => repository.signs;
}
