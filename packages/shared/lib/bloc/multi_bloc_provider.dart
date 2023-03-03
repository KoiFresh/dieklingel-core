import 'package:flutter/cupertino.dart';
import 'package:nested/nested.dart';

import 'bloc_provider.dart';
import 'bloc.dart';

class MultiBlocProvider<T extends Bloc> extends SingleChildStatelessWidget {
  final List<BlocProvider<T>> blocs;

  const MultiBlocProvider({
    required this.blocs,
    super.child,
    super.key,
  });

  @override
  Widget buildWithChild(BuildContext context, Widget? child) {
    return Nested(
      children: blocs,
      child: child,
    );
  }
}