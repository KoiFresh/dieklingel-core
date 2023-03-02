import 'dart:io';

import 'package:dieklingel_core_shared/flutter_shared.dart';
import 'package:flutter/material.dart';
import 'package:lottie/lottie.dart';
import 'package:path/path.dart' as p;

import '../blocs/screensaver_view_bloc.dart';
import '../models/screensaver_options.dart';
import '../models/sign_options.dart';

class ScreensaverView extends StatelessWidget {
  const ScreensaverView({super.key});

  Widget _lottie(BuildContext context, String path) {
    File lottiefile = File(path);

    return Lottie.file(
      lottiefile,
      fit: BoxFit.cover,
    );
  }

  Widget _html(BuildContext context, String path) {
    File htmlfile = File(path);

    return FutureBuilder(
      future: htmlfile.readAsString(),
      builder: (context, snapshot) {
        if (!snapshot.hasData) {
          return const Center(
            child: CircularProgressIndicator(),
          );
        }
        // TODO(KoiFresh): add native support
        return const Text("HTML is not supported");
      },
    );
  }

  Widget _image(BuildContext context, String path) {
    String fullPath = p.join(
      Platform.environment["SNAP_REAL_HOME"] ??
          Platform.environment["HOME"] ??
          "",
      "dieklingel",
      path,
    );

    return SizedBox(
      width: double.infinity,
      height: double.infinity,
      child: Image.file(
        File(fullPath),
        fit: BoxFit.cover,
      ),
    );
  }

  Widget _text(BuildContext context, String path) {
    return Center(
      child: Text("Missconfigured: $path"),
    );
  }

  @override
  Widget build(BuildContext context) {
    return GestureDetector(
      onTap: () {
        context
            .bloc<MqttClientBloc>()
            .message
            .add(const MapEntry("display/state", "on"));
      },
      child: StreamBuilder(
        stream: context.bloc<ScreensaverViewBloc>().options,
        builder: (context, AsyncSnapshot<ScreensaverOptions> snapshot) {
          if (!snapshot.hasData || snapshot.data?.type == SignType.none) {
            return const Center(
              child: CircularProgressIndicator(),
            );
          }

          ScreensaverOptions options = snapshot.data!;

          Widget child;
          switch (options.type) {
            case SignType.lottie:
              child = _lottie(context, options.file);
              break;
            case SignType.html:
              child = _html(context, options.file);
              break;
            case SignType.image:
              child = _image(context, options.file);
              break;
            default:
              child = _text(context, options.file);
          }

          return SizedBox(
            width: double.infinity,
            height: double.infinity,
            child: child,
          );
        },
      ),
    );
  }
}
