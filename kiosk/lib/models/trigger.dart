enum Trigger {
  unknown,
  ring,
  movement,
  unlock;

  static Trigger fromString(String action) {
    switch (action.toLowerCase()) {
      case "ring":
        return Trigger.ring;
      case "movement":
        return Trigger.movement;
      case "unlock":
        return Trigger.unlock;
    }
    return Trigger.unknown;
  }
}
