import 'dart:io';
import 'package:audioplayers/audioplayers.dart';
import 'package:flutter/material.dart';
import 'package:gui/components/rfw_library/rfw_library.dart';
import 'package:lottie/lottie.dart';

import 'package:path/path.dart' as p;
import 'package:rfw/formats.dart';
import 'package:rfw/rfw.dart';
import 'package:rxdart/streams.dart';
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
  final AudioPlayer _player = AudioPlayer();
  final Runtime _runtime = Runtime();
  final DynamicContent _content = DynamicContent();

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

  Widget _native(BuildContext context) {
    String path = p.join(
      Platform.environment["SNAP_REAL_HOME"] ??
          Platform.environment["HOME"] ??
          "",
      "dieklingel",
      widget.options.file,
    );
    File dartfile = File(path);

    return FutureBuilder(
      future: dartfile.readAsString(),
      builder: ((context, snapshot) {
        if (!snapshot.hasData) {
          return const Center(
            child: CircularProgressIndicator(),
          );
        }
        String dart = snapshot.data!;

        RemoteWidgetLibrary rwidget;
        try {
          rwidget = parseLibraryFile(dart);
        } on ParserException catch (exception) {
          stdout.writeln(
            "Error on creating Sing: ${widget.options.identifier}: ${exception.toString()}",
          );
          return Text(exception.toString());
        }
        _runtime.update(
          const LibraryName(<String>['core', 'widgets']),
          createCoreWidgets(),
        );
        _runtime.update(
          const LibraryName(<String>['dieKlingel']),
          createDieklingelWidgets(),
        );
        _runtime.update(
          const LibraryName(<String>['main']),
          rwidget,
        );

        return RemoteWidget(
          runtime: _runtime,
          widget: const FullyQualifiedWidgetName(
            LibraryName(['main']),
            'root',
          ),
          data: _content,
          onEvent: (eventName, eventArguments) {
            if (eventName == "ring") {
              _onClick();
            }
          },
        );
      }),
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

  Future<void> _onClick() async {
    widget.onTap?.call(widget.options.identifier);

    _play();

    if (widget.options.type == SignType.lottie) {
      await _controller.forward(from: 0);
    }
  }

  Future<void> _play() async {
    String? sound = widget.options.sound;
    if (sound == null || sound.isEmpty) {
      return;
    }
    String path = p.join(
      Platform.environment["SNAP_REAL_HOME"] ??
          Platform.environment["HOME"] ??
          "",
      "dieklingel",
      sound,
    );
    await _player.setSourceDeviceFile(path);
    await _player.stop();
    await _player.resume();
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
      case SignType.native:
        child = _native(context);
        break;
      default:
        child = _text(context);
        break;
    }

    return GestureDetector(
      onTap: () => _onClick(),
      child: Container(
        color: Colors.red,
        child: child,
      ),
    );
  }
}
