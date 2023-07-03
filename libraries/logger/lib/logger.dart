library logger;

import 'dart:io';

enum LogLevel {
  info,
  warn,
  error,
}

class Logger {
  static void info(String message) => log(LogLevel.info, message);

  static void warn(String message) => log(LogLevel.warn, message);

  static void error(String message) => log(LogLevel.error, message);

  static void log(LogLevel level, String message) {
    switch (level) {
      case LogLevel.info:
        stdout.writeln("[ info  ] $message");
        break;
      case LogLevel.warn:
        stdout.writeln("[ warn  ] $message");
        break;
      case LogLevel.error:
        stdout.writeln("[ error ] $message");
        break;
    }
  }
}
