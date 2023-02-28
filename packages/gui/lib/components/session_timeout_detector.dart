import 'dart:async';

import 'package:flutter/gestures.dart';
import 'package:flutter/material.dart';

class SessionTimeoutDetector extends StatefulWidget {
  final Widget child;
  final Duration timeout;
  final void Function()? onInactivity;
  final void Function()? onActivity;

  const SessionTimeoutDetector({
    required this.child,
    required this.timeout,
    this.onActivity,
    this.onInactivity,
    super.key,
  });

  @override
  State<StatefulWidget> createState() => _SessionTimeoutDetector();
}

class _SessionTimeoutDetector extends State<SessionTimeoutDetector> {
  Timer? _timer;

  @override
  void initState() {
    _onInteraction();
    super.initState();
  }

  void _onInteraction() {
    if (!(_timer?.isActive ?? true)) {
      widget.onActivity?.call();
    }
    _timer?.cancel();
    _timer = Timer(widget.timeout, _onTimeout);
  }

  void _onTimeout() {
    _timer?.cancel();
    widget.onInactivity?.call();
  }

  @override
  Widget build(BuildContext context) {
    return Listener(
      onPointerSignal: (PointerSignalEvent event) => _onInteraction(),
      onPointerDown: (event) => _onInteraction(),
      onPointerUp: (event) => _onInteraction(),
      onPointerMove: (event) => _onInteraction(),
      onPointerHover: (event) => _onInteraction(),
      child: widget.child,
    );
  }

  @override
  void dispose() {
    _timer?.cancel();
    super.dispose();
  }
}
