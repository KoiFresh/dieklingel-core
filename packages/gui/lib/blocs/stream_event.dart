abstract class StreamEvent {}

abstract class ActivityState extends StreamEvent {}

class InactiveState extends ActivityState {
  @override
  String toString() {
    return "inactive";
  }
}

class ActiveState extends ActivityState {
  @override
  String toString() {
    return "active";
  }
}

abstract class DisplayState extends StreamEvent {
  bool get isOn;
}

class DisplayOnState extends DisplayState {
  @override
  bool get isOn => true;

  @override
  String toString() {
    return "on";
  }
}

class DisplayOffState extends DisplayState {
  @override
  bool get isOn => false;

  @override
  String toString() {
    return "off";
  }
}
