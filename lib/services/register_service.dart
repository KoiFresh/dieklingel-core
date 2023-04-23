import 'package:dieklingel_core/repositories/register_repository.dart';
import 'package:mqtt/mqtt.dart';

class ReigsterService {
  final Client client;
  final RegisterRepository repository;

  ReigsterService(this.client, this.repository);
}
