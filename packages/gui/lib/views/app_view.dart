import 'package:dieklingel_core_shared/flutter_shared.dart';
import 'package:flutter/material.dart';
import 'package:get_it/get_it.dart';
import 'package:gui/blocs/app_state_bloc.dart';
import 'package:gui/blocs/stream_event.dart';
import 'package:gui/components/activity_listener.dart';

import '../blocs/app_view_bloc.dart';
import '../utils/touch_scroll_behavior.dart';
import 'home_view.dart';

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  ThemeData _darkTheme(BuildContext context) {
    ThemeData themeData = ThemeData.dark();

    return themeData;
  }

  Widget _app(BuildContext context) {
    return MaterialApp(
      themeMode: ThemeMode.dark,
      darkTheme: _darkTheme(context),
      scrollBehavior: TouchScrollBehavior(),
      home: ActivityListener(
        onActivity: () {
          GetIt.I<AppStateBloc>().activity.add(ActiveState());
        },
        child: const HomeView(),
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
