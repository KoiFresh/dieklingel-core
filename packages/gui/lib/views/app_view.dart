import 'package:dieklingel_core_shared/flutter_shared.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';

import '../blocs/app_view_bloc.dart';
import '../utils/touch_scroll_behavior.dart';
import 'home_view.dart';

class MyApp extends StatelessWidget {
  const MyApp({super.key});

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
            child: CupertinoApp(
              scrollBehavior: TouchScrollBehavior(),
              home: const HomeView(),
            ),
          ),
        );
      },
    );
  }
}
