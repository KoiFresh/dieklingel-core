import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';

import '../blocs/sign_list_view_bloc.dart';
import '../components/bell_sign.dart';
import '../models/sign.dart';
import '../states/sign_list_state.dart';

class SignListView extends StatelessWidget {
  const SignListView({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: BlocBuilder<SignListViewBloc, SignListState>(
        builder: (context, state) {
          if (state.isLoading) {
            return const Center(
              child: CircularProgressIndicator(),
            );
          }

          return ListView.builder(
            itemCount: state.signs.length,
            itemBuilder: (context, index) {
              Sign sign = state.signs[index];

              return BellSign(
                sign: sign,
                onTap: (identifier) {
                  context.read<SignListViewBloc>().ring(sign);
                },
              );
            },
          );

          ListView(
            children: state.signs.map((e) => BellSign(sign: e)).toList(),
          );
        },
      ),
    );
  }
}
