library rfw_library;

import 'package:flutter/material.dart';
import 'package:rfw/rfw.dart';

import 'components/simple_sign.dart';

WidgetLibrary createDieklingelWidgets() {
  return LocalWidgetLibrary(
    {
      "SimpleSign": (BuildContext context, DataSource source) {
        String text = source.v<String>(<Object>["text"]) ?? "dieKlingel";
        double height = source.v<double>(<Object>["height"]) ?? 1024.0;
        Color color = ArgumentDecoders.color(source, ['color']) ??
            const Color(0xFFFFA424);

        return SimpleSign(
          text: text,
          height: height,
          backgroundColor: color,
          onTap: source.voidHandler(['onTap']),
        );
      }
    },
  );
}
