import 'package:shelf/shelf.dart';
import 'package:shelf_router/shelf_router.dart';

class CameraService {
  Handler get handler {
    final router = Router();

    router.get("/snapshot", (Request request) {
      return Response(501, body: "Snapshot is not implemented!");
    });

    return router;
  }
}
