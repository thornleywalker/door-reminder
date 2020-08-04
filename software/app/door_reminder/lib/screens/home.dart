import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';

import 'package:door_reminder/singleton.dart';
import 'package:door_reminder/login_services/authentication.dart';
import 'package:door_reminder/hamburger_menu.dart';
import 'package:door_reminder/account_menu.dart';

class HomePage extends StatefulWidget {
  HomePage({BaseAuth auth, VoidCallback logoutCallback}) {
    singleton.setAuth(auth);
    singleton.setLogoutCallback(logoutCallback);
  }

  final singleton = Singleton();

  @override
  State<StatefulWidget> createState() => new _HomePageState();
}

class _HomePageState extends State<HomePage> {
  var singleton = Singleton();
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("Doorsi"),
        actions: <Widget>[
          AccountMenu(),
        ],
      ),
      drawer: HamburgerMenu(),
      body: Column(
        children: <Widget>[
          FutureBuilder(
            future: singleton.testMethod(),
            builder: (context, AsyncSnapshot<String> val) {
              if (val.hasData)
                return Text(val.data);
              else
                return Text('loading');
            },
          )
        ],
      ),
    );
  }
}
