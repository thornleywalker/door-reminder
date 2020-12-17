import 'package:flutter/material.dart';
import 'package:door_reminder/widgets/hamburger_menu.dart';

class HelpRoute extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Help'),
      ),
      drawer: HamburgerMenu(),
      body: Center(
        child: Column(
            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
            crossAxisAlignment: CrossAxisAlignment.center,
            children: <Widget>[
              Text('Contact Email: developer@tthbgroup.org'),
            ]),
      ),
    );
  }
}
