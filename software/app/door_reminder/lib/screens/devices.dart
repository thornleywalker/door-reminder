import 'package:flutter/material.dart';
import 'package:door_reminder/singleton.dart';
import 'package:door_reminder/hamburger_menu.dart';

class DevicesRoute extends StatelessWidget {
  var singleton = Singleton();
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Devices'),
      ),
      drawer: HamburgerMenu(),
      body: Center(
        child: Column(
          children: <Widget>[
            Text('current devices list'),
            RaisedButton(
              child: Text('add device'),
              onPressed: () async {
                singleton.addDevice('new-device');
              },
            ),
          ],
        ),
      ),
    );
  }
}