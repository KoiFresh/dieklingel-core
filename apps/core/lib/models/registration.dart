class Registration {
  final int id;
  final String sign;
  final String token;
  final DateTime created;

  Registration({
    this.id = 0,
    required this.sign,
    required this.token,
    required this.created,
  });
}
