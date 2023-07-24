import 'dart:convert';

import 'package:shelf/shelf.dart';
import 'package:shelf_router/shelf_router.dart';

import '../models/sign.dart';
import '../repositories/sign_repository.dart';

class SignService {
  SignRepository signRepository;

  SignService(this.signRepository);

  Handler get handler {
    final Router router = Router();

    router.get("/", (Request request) async {
      List<Sign> signs = await signRepository.fetchAllSigns();

      return Response.ok(
        jsonEncode(signs.map((e) => e.identifier).toList()),
        headers: {
          "Content-Type": "application/json",
        },
      );
    });

    return router;
  }
}
