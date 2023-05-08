import 'package:flutter/material.dart';

import '../components/bell_sign.dart';

class SignListView extends StatelessWidget {
  const SignListView({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: ListView(
        children: [
          BellSign(),
        ],
      ),
    );
  }
}
