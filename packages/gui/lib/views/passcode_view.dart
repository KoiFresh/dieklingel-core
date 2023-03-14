import 'package:dieklingel_core_shared/flutter_shared.dart';
import 'package:flutter/material.dart';
import 'package:gui/blocs/passcode_view_bloc.dart';

class PasscodeView extends StatefulWidget {
  const PasscodeView({super.key});

  @override
  State<StatefulWidget> createState() => _PasscodeView();
}

class _PasscodeView extends State<PasscodeView> {
  final TextEditingController _controller = TextEditingController();
  final List<String> _numbers = [
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
  ];

  @override
  void initState() {
    _numbers.shuffle();
    super.initState();
  }

  EdgeInsets get _buttonPadding {
    return const EdgeInsets.all(4.0);
  }

  double get _buttonWidth {
    return kMinInteractiveDimension * 1.6;
  }

  double get _pincodeWidth {
    return _buttonWidth * 3 - _buttonPadding.right - _buttonPadding.left;
  }

  Widget _button(BuildContext context, Widget child, {void Function()? onTap}) {
    return Container(
      padding: _buttonPadding,
      width: _buttonWidth,
      height: _buttonWidth,
      child: MaterialButton(
        padding: EdgeInsets.zero,
        onPressed: onTap,
        child: child,
      ),
    );
  }

  Widget _textfield(BuildContext context) {
    return SizedBox(
      width: _pincodeWidth,
      child: TextField(
        keyboardType: TextInputType.none,
        controller: _controller,
        textAlign: TextAlign.center,
        obscureText: true,
        onSubmitted: (text) {
          context.bloc<PasscodeViewBloc>().sendPasscode(_controller.text);
          _controller.clear();
        },
      ),
    );
  }

  Widget _pinfield(BuildContext context) {
    return Column(
      children: [
        Row(
          mainAxisSize: MainAxisSize.min,
          children: [
            _button(
              context,
              Text(_numbers[1]),
              onTap: () => _controller.text += _numbers[1],
            ),
            _button(
              context,
              Text(_numbers[2]),
              onTap: () => _controller.text += _numbers[2],
            ),
            _button(
              context,
              Text(_numbers[3]),
              onTap: () => _controller.text += _numbers[3],
            ),
          ],
        ),
        Row(
          mainAxisSize: MainAxisSize.min,
          children: [
            _button(
              context,
              Text(_numbers[4]),
              onTap: () => _controller.text += _numbers[4],
            ),
            _button(
              context,
              Text(_numbers[5]),
              onTap: () => _controller.text += _numbers[5],
            ),
            _button(
              context,
              Text(_numbers[6]),
              onTap: () => _controller.text += _numbers[6],
            ),
          ],
        ),
        Row(
          mainAxisSize: MainAxisSize.min,
          children: [
            _button(
              context,
              Text(_numbers[7]),
              onTap: () => _controller.text += _numbers[7],
            ),
            _button(
              context,
              Text(_numbers[8]),
              onTap: () => _controller.text += _numbers[8],
            ),
            _button(
              context,
              Text(_numbers[9]),
              onTap: () => _controller.text += _numbers[9],
            ),
          ],
        ),
        Row(
          mainAxisSize: MainAxisSize.min,
          children: [
            _button(
              context,
              const Icon(Icons.vpn_key_outlined),
              onTap: () {
                PasscodeViewBloc bloc = context.bloc<PasscodeViewBloc>();
                bloc.sendPasscode(_controller.text);
                _controller.clear();

                setState(() {
                  _numbers.shuffle();
                });
              },
            ),
            _button(
              context,
              Text(_numbers[0]),
              onTap: () => _controller.text += _numbers[0],
            ),
            _button(
              context,
              const Icon(Icons.backspace_outlined),
              onTap: () {
                if (_controller.text.isEmpty) {
                  return;
                }
                String text = _controller.text;
                text = text.substring(0, text.length - 1);
                _controller.text = text;
              },
            ),
          ],
        )
      ],
    );
  }

  @override
  Widget build(BuildContext context) {
    return Theme(
      data: ThemeData.dark(),
      child: Scaffold(
        body: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              _textfield(context),
              _pinfield(context),
            ],
          ),
        ),
      ),
    );
  }
}
