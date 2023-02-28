import 'package:flutter/material.dart';

class TapDetector extends StatefulWidget {
  final Widget? child;
  final void Function()? onTap;

  const TapDetector({
    this.onTap,
    this.child,
    super.key,
  });

  @override
  State<StatefulWidget> createState() => _TapDetector();
}

class _TapDetector extends State<TapDetector> {
  Offset position = const Offset(0, 0);

  @override
  Widget build(BuildContext context) {
    return Listener(
      onPointerDown: (PointerDownEvent event) {
        position = event.position;
      },
      onPointerUp: (PointerUpEvent event) {
        if (event.position != position) {
          return;
        }
        widget.onTap?.call();
      },
      child: widget.child,
    );
  }
}
