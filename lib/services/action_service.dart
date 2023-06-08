import 'dart:convert';

import 'package:shelf/shelf.dart';
import 'package:shelf_router/shelf_router.dart';

import '../models/action.dart';
import '../repositories/action_repository.dart';

class ActionService {
  final ActionRepository actionRepository;

  ActionService(this.actionRepository);

  Handler get handler {
    final Router router = Router();

    router.get("/", (Request request) async {
      List<Action> actions = await actionRepository.fetchAllActions();
      List<Map<String, dynamic>> body = actions.map((e) => e.toMap()).toList();

      return Response.ok(
        jsonEncode(body),
        headers: {
          "Content-Type": "application/json",
        },
      );
    });

    return router;
  }
}
