import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';

import '../blocs/app_view_bloc.dart';
import '../states/app_state.dart';
import 'passcode_view.dart';
import 'sign_list_view.dart';

class HomeView extends StatefulWidget {
  const HomeView({super.key});

  @override
  State<StatefulWidget> createState() => _HomeView();
}

class _HomeView extends State<HomeView> {
  final _controller = PageController();

  @override
  Widget build(BuildContext context) {
    return BlocListener<AppViewBloc, AppState>(
      listener: (BuildContext context, AppState state) {
        if (state.display.isOff) {
          _controller.jumpToPage(0);
        }
      },
      child: PageView(
        controller: _controller,
        children: const [
          SignListView(),
          PasscodeView(),
        ],
      ),
    );
  }

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }
}
