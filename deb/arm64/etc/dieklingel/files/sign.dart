import core.widgets;
import dieKlingel;
// The "widget" keyword is used to define a new widget constructor.
// The "root" widget is specified as the one to render in the build
// method below.

widget root = SimpleSign(
  text: "Fam. Schoch",
  height: 999.0,
  onTap: event "ring" {},
);