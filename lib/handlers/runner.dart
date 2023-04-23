library runner;

import 'package:flutter/material.dart';

import 'handler.dart';

Runner run(Handler handler) {
  return Runner(handler);
}

class Runner {
  final List<Handler> _handlers = [];

  Runner(Handler app) {
    and(app);
  }

  Runner and([Handler? app]) {
    if (app != null) {
      _handlers.add(app);
      app.run();
    }
    return this;
  }

  void app(Widget app) {
    runApp(app);
  }
}
