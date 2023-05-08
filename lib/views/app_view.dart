import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';

import '../blocs/passcode_view_bloc.dart';
import '../blocs/sign_list_view_bloc.dart';
import '../repositories/action_repository.dart';
import '../repositories/sign_repository.dart';
import '../utils/touch_scroll_behavior.dart';
import 'passcode_view.dart';
import 'sign_list_view.dart';

class App extends StatelessWidget {
  const App({super.key});

  @override
  Widget build(BuildContext context) {
    return Container(
      color: Colors.black,
      //adding: snapshot.data,
      child: ClipRRect(
        clipBehavior: Clip.antiAliasWithSaveLayer,
        borderRadius: BorderRadius.circular(20),
        child: const _Viewport(),
      ),
    );
  }
}

class _Viewport extends StatelessWidget {
  const _Viewport();

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
        themeMode: ThemeMode.dark,
        darkTheme: ThemeData.dark(useMaterial3: true),
        scrollBehavior: TouchScrollBehavior(),
        home: MultiBlocProvider(
          providers: [
            BlocProvider(
              create: (_) => PasscodeViewBloc(
                context.read<ActionRepository>(),
              ),
            ),
            BlocProvider(
              create: (_) => SignListViewBloc(
                context.read<SignRepository>(),
                context.read<ActionRepository>(),
              ),
            )
          ],
          child: PageView(
            children: const [
              SignListView(),
              PasscodeView(),
            ],
          ),
        ));
  }
}
