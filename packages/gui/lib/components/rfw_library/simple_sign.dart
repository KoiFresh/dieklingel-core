import 'package:flutter/material.dart';

class SimpleSign extends StatefulWidget {
  final String text;
  final double height;
  final Color backgroundColor;
  final void Function()? onTap;

  const SimpleSign({
    required this.text,
    required this.height,
    required this.backgroundColor,
    this.onTap,
    super.key,
  });

  @override
  State<StatefulWidget> createState() => _SimpleSign();
}

class _SimpleSign extends State<SimpleSign>
    with SingleTickerProviderStateMixin {
  late final AnimationController _controller = AnimationController(
    vsync: this,
    duration: const Duration(seconds: 1),
  );

  @override
  Widget build(BuildContext context) {
    return GestureDetector(
      onTap: () async {
        widget.onTap?.call();
        await _controller.forward();
        await _controller.reverse();
      },
      child: Container(
        width: double.infinity,
        color: widget.backgroundColor,
        height: widget.height,
        padding: EdgeInsets.all(
          widget.height / 100,
          //MediaQuery.of(context).size.height * widget.height / 100 / 10,
        ),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.spaceAround,
          children: [
            Padding(
              padding: EdgeInsets.only(top: widget.height / 5),
              child: Stack(
                alignment: Alignment.center,
                clipBehavior: Clip.none,
                children: [
                  Positioned(
                    top: widget.height / 35,
                    child: RotationTransition(
                      alignment: Alignment.topCenter,
                      turns: Tween(begin: 0.0, end: -0.02)
                          .chain(CurveTween(curve: Curves.elasticIn))
                          .animate(_controller),
                      child: Image.asset(
                        "assets/images/clapper.png",
                        fit: BoxFit.fitHeight,
                        height: widget.height / 4,
                      ),
                    ),
                  ),
                  Positioned(
                    child: RotationTransition(
                      alignment: Alignment.topCenter,
                      turns: Tween(begin: 0.0, end: 0.03)
                          .chain(CurveTween(curve: Curves.elasticIn))
                          .animate(_controller),
                      child: Image.asset(
                        "assets/images/bell.png",
                        fit: BoxFit.fitHeight,
                        height: widget.height / 4,
                      ),
                    ),
                  ),
                ],
              ),
            ),
            Text(
              widget.text,
              style: const TextStyle(fontSize: 55),
            ),
          ],
        ),
      ),
    );
  }
}
