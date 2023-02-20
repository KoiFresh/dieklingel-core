import 'dart:async';

import 'package:dieklingel_core_shared/shared.dart';
import 'package:flutter/cupertino.dart';
import '../blocs/mqtt_state_mixin.dart';
import '../blocs/screensaver_view_bloc.dart';
import '../blocs/sign_view_bloc.dart';
import '../blocs/stream_event.dart';
import 'passcode_view.dart';
import 'screensaver_view.dart';
import 'sign_view.dart';

class HomeView extends StatefulWidget {
  final Map<String, dynamic> config;
  const HomeView({
    this.config = const {},
    super.key,
  });

  @override
  State<StatefulWidget> createState() => _HomeView();
}

class _HomeView extends State<HomeView> with MqttStateMixin {
  late final StreamSubscription _activity;
  final _controller = PageController();

  @override
  void initState() {
    _activity = activity.listen((event) {
      if (event is InactiveState) {
        _controller.jumpToPage(0);
      }
    });

    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    return CupertinoPageScaffold(
      child: MultiBlocProvider(
        blocs: [
          BlocProvider(bloc: SignViewBloc()),
          BlocProvider(bloc: ScreensaverViewBloc()),
        ],
        child: StreamBuilder(
          stream: display,
          builder: (context, AsyncSnapshot<DisplayState> snapshot) {
            if (snapshot.data is DisplayOffState) {
              return const ScreensaverView();
            }
            return PageView(
              controller: _controller,
              children: const [
                SignView(),
                PasscodeView(),
              ],
            );
          },
        ),
      ),
    );
  }

  @override
  void dispose() {
    _activity.cancel();
    super.dispose();
  }
}
