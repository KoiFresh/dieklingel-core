import 'dart:io';

import 'package:audioplayers/audioplayers.dart';
import 'package:flutter/material.dart';
import 'package:logger/logger.dart';
import 'package:rfw/formats.dart';
import 'package:rfw/rfw.dart';
import 'package:ui/ui.dart';
import 'package:path/path.dart' as path;

import '../models/sign.dart';

class BellSign extends StatefulWidget {
  final Sign sign;
  final void Function(String identifier)? onTap;

  const BellSign({
    required this.sign,
    this.onTap,
    super.key,
  });

  @override
  State<StatefulWidget> createState() => _Sign();
}

class _Sign extends State<BellSign> {
  final AudioPlayer _player = AudioPlayer();
  final Runtime _runtime = Runtime();
  final DynamicContent _content = DynamicContent();

  Widget _native(BuildContext context) {
    return FutureBuilder(
      future: widget.sign.interface.exists(),
      builder: (context, snapshot) {
        bool? fileExists = snapshot.data;
        if (fileExists == null) {
          return const Center(
            child: CircularProgressIndicator(),
          );
        }

        if (!fileExists) {
          return const Row(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              Icon(
                Icons.error_outline,
                color: Colors.red,
              ),
              SizedBox(width: 10),
              Text("Houh, looks like something went wron!"),
            ],
          );
        }

        return FutureBuilder(
          future: widget.sign.interface.readAsString(),
          builder: (context, snapshot) {
            if (snapshot.hasError) {
              return const Center(
                child: Icon(
                  Icons.sync_problem,
                  color: Colors.red,
                ),
              );
            }

            if (!snapshot.hasData) {
              return const Center(
                child: CircularProgressIndicator(),
              );
            }
            String rfw = snapshot.data!;

            RemoteWidgetLibrary rwidget;
            try {
              rwidget = parseLibraryFile(rfw);
            } on ParserException catch (exception) {
              stdout.writeln(
                "Error on creating Sing: ${widget.sign.identifier}: ${exception.toString()}",
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
                  _play();
                }
              },
            );
          },
        );
      },
    );
  }

  Future<void> _onClick() async {
    widget.onTap?.call(widget.sign.identifier);
  }

  Future<void> _play() async {
    if (!(await widget.sign.audio.exists())) {
      Logger.error(
        "The requested file '${widget.sign.audio.path}' does not exist.",
      );
      return;
    }
    await _player.setSourceDeviceFile(path.absolute(widget.sign.audio.path));
    await _player.stop();
    await _player.resume();
  }

  @override
  Widget build(BuildContext context) {
    return GestureDetector(
      onTap: () => _onClick(),
      child: Container(
        child: _native(context),
      ),
    );
  }
}
