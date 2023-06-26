import 'package:flutter/gestures.dart';
import 'package:flutter/material.dart';

class ActivityListener extends StatelessWidget {
  final Widget child;
  final void Function()? onActivity;

  const ActivityListener({
    required this.child,
    this.onActivity,
    super.key,
  });

  void _onInteraction() {
    onActivity?.call();
  }

  @override
  Widget build(BuildContext context) {
    return Listener(
      onPointerSignal: (PointerSignalEvent event) => _onInteraction(),
      onPointerDown: (event) => _onInteraction(),
      onPointerUp: (event) => _onInteraction(),
      onPointerMove: (event) => _onInteraction(),
      onPointerHover: (event) => _onInteraction(),
      child: child,
    );
  }
}
