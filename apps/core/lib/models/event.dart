import 'package:yaml/yaml.dart';

import 'trigger.dart';

class Event {
  final DateTime created;
  final String hash;
  final dynamic content;

  Event({
    required this.created,
    required this.hash,
    required this.content,
  });
}
