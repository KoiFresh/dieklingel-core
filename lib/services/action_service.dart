import 'dart:convert';

import 'package:blueprint/blueprint.dart';
import 'package:shelf/shelf.dart';
import 'package:shelf_router/shelf_router.dart';
import 'package:shell/shell.dart';

import '../models/action.dart';
import '../repositories/action_repository.dart';

class ActionService {
  final ActionRepository actionRepository;

  ActionService(this.actionRepository);

  Handler get handler {
    final Router router = Router();

    router.get("/", (Request request) async {
      List<Action> actions;
      if (request.requestedUri.queryParameters.containsKey("trigger")) {
        actions = await actionRepository.fetchActionsFor(
          request.requestedUri.queryParameters["trigger"]!,
        );
      } else {
        actions = await actionRepository.fetchAllActions();
      }
      List<Map<String, dynamic>> body = actions.map((e) => e.toMap()).toList();

      return Response.ok(
        jsonEncode(body),
        headers: {
          "Content-Type": "application/json",
        },
      );
    });

    router.post("/", (Request request) async {
      Map<String, dynamic> body;
      String trigger;
      Map<String, dynamic> environment;
      try {
        body = jsonDecode(await request.readAsString());

        matchMap(
          body,
          {
            "trigger": StringF,
            "environment": MapOrNull,
          },
          throwable: true,
        );

        trigger = body["trigger"];
        environment = body["environment"] ?? {};
      } on FormatException catch (error) {
        return Response.badRequest(body: error.toString());
      } on TypeDoesNotMatch catch (error) {
        return Response.badRequest(body: error.toString());
      }

      List<Action> actions = await actionRepository.fetchActionsFor(trigger);

      for (Action action in actions) {
        Shell shell = Shell(environment: environment.cast<String, String>());

        await shell.run("eval", arguments: [action.lane]);
      }

      return Response.ok(
        jsonEncode(actions.map((e) => e.toMap()).toList()),
        headers: {
          "Content-Type": "application/json",
        },
      );
    });

    return router;
  }
}
