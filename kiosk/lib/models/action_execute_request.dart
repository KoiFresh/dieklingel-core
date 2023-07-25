class ActionExecuteRequest {
  String pattern;
  Map<String, String> environment;

  ActionExecuteRequest(
    this.pattern, {
    this.environment = const {},
  });

  Map<String, dynamic> toMap() {
    return {
      "pattern": pattern,
      "environment": environment,
    };
  }
}
