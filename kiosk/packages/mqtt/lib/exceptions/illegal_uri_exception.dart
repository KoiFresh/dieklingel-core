class IllegalUriException implements Exception {
  final String reason;
  final Uri uri;

  const IllegalUriException(this.uri, this.reason);
}
