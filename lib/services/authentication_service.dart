import 'package:shelf/shelf.dart';
import 'package:shelf_router/shelf_router.dart';

class AuthenticationService {
  final Map<String, Handler> routes;

  AuthenticationService(this.routes);

  Handler get handler {
    final router = Router();

    for (MapEntry<String, Handler> entry in routes.entries) {
      final String prefix = entry.key;
      final Handler handler = entry.value;

      router.mount(prefix, handler);
    }

    return const Pipeline().addMiddleware(_authenticate()).addHandler(router);
  }

  Middleware _authenticate() {
    return (Handler innerHandler) {
      return (Request request) {
        return Future.sync(() => innerHandler(request));
      };
    };
  }
}
