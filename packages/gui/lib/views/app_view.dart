import 'package:dieklingel_core_shared/flutter_shared.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:gui/blocs/mqtt_state_mixin.dart';
import 'package:gui/blocs/stream_event.dart';
import 'package:gui/components/session_timeout_detector.dart';

import '../blocs/app_view_bloc.dart';
import '../utils/touch_scroll_behavior.dart';
import 'home_view.dart';

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  Widget _app(BuildContext context) {
    return CupertinoApp(
      scrollBehavior: TouchScrollBehavior(),
      home: StreamBuilder(
        stream: context.bloc<AppViewBloc>().timeout,
        builder: (context, snapshot) {
          Duration duration = snapshot.data ?? const Duration(seconds: 30);

          return SessionTimeoutDetector(
            timeout: duration,
            onActivity: () {
              context.bloc<AppViewBloc>().setActivityState(ActiveState());
            },
            onInactivity: () {
              context.bloc<AppViewBloc>().setActivityState(InactiveState());
            },
            child: const HomeView(),
          );
        },
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return StreamBuilder(
      stream: context.bloc<AppViewBloc>().clip,
      builder: (
        BuildContext context,
        AsyncSnapshot<EdgeInsets> snapshot,
      ) {
        return Container(
          color: Colors.black,
          padding: snapshot.data,
          child: ClipRRect(
            clipBehavior: Clip.antiAliasWithSaveLayer,
            borderRadius: BorderRadius.circular(20),
            child: _app(context),
          ),
        );
      },
    );
  }
}
