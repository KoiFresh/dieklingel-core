import 'dart:io';
import 'package:flutter/material.dart';
import 'package:lottie/lottie.dart';

import 'package:path/path.dart' as p;
import '../models/sign_options.dart';

class Sign extends StatefulWidget {
  final SignOptions options;
  final void Function(String hash)? onTap;

  const Sign({
    required this.options,
    this.onTap,
    super.key,
  });

  @override
  State<StatefulWidget> createState() => _Sign();
}

class _Sign extends State<Sign> with SingleTickerProviderStateMixin {
  late final AnimationController _controller = AnimationController(vsync: this);

  Widget _lottie(BuildContext context) {
    String path = p.join(
      Platform.environment["SNAP_REAL_HOME"] ??
          Platform.environment["HOME"] ??
          "",
      "dieklingel",
      widget.options.file,
    );
    File lottiefile = File(path);

    return Lottie.file(
      lottiefile,
      controller: _controller,
      fit: BoxFit.contain,
      onLoaded: (composition) {
        _controller.duration = composition.duration;
      },
    );
  }

  Widget _html(BuildContext context) {
    String path = p.join(
      Platform.environment["SNAP_REAL_HOME"] ??
          Platform.environment["HOME"] ??
          "",
      "dieklingel",
      widget.options.file,
    );
    File htmlfile = File(path);

    return FutureBuilder(
      future: htmlfile.readAsString(),
      builder: (context, snapshot) {
        if (!snapshot.hasData) {
          return const Center(
            child: CircularProgressIndicator(),
          );
        }
        // TODO(KoiFresh): add native support instead of html
        return const Text("HTML is not supported");
      },
    );
  }

  Widget _image(BuildContext context) {
    String path = p.join(
      Platform.environment["SNAP_REAL_HOME"] ??
          Platform.environment["HOME"] ??
          "",
      "dieklingel",
      widget.options.file,
    );

    return Image.file(
      File(path),
      fit: BoxFit.contain,
    );
  }

  Widget _text(BuildContext context) {
    return Center(
      child: Text("Missconfigured: ${widget.options.identifier}"),
    );
  }

  @override
  Widget build(BuildContext context) {
    Widget child;

    switch (widget.options.type) {
      case SignType.lottie:
        child = _lottie(context);
        break;
      case SignType.html:
        child = _html(context);
        break;
      case SignType.image:
        child = _image(context);
        break;
      default:
        child = _text(context);
        break;
    }

    return GestureDetector(
      onTap: () async {
        widget.onTap?.call(widget.options.identifier);

        if (widget.options.type == SignType.lottie) {
          await _controller.forward(from: 0);
        }
      },
      child: Container(
        color: Colors.red,
        child: child,
      ),
    );
  }
}
