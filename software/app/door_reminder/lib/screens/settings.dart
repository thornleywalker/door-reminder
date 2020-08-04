import 'package:firebase_auth/firebase_auth.dart';
import 'package:flutter/material.dart';
import 'package:door_reminder/hamburger_menu.dart';
import 'package:door_reminder/login_services/sign_in.dart';
import 'package:door_reminder/login_services/login_page.dart';
import 'package:door_reminder/screens/home.dart';
import 'package:door_reminder/login_services/authentication.dart';
import 'package:door_reminder/account_menu.dart';

class SettingsRoute extends StatefulWidget {
  SettingsRoute({Key key, this.title, this.userId}) : super(key: key);
  final String title;
  final String userId;

  @override
  State<StatefulWidget> createState() => new _SettingsRouteState();
}

class _SettingsRouteState extends State<SettingsRoute> {
  _SettingsRouteState({this.user});
  final FirebaseUser user;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Settings'),
        actions: <Widget>[
          AccountMenu(),
        ],
      ),
      drawer: HamburgerMenu(),
      body: Center(
          child: Column(
        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
        children: <Widget>[
          ListTile(
            title: Text("App Color: based on my rides"),
            onTap: () {},
          ),
          ListTile(
            title: Text('App Color: Based on my rides'),
          ),
          ListTile(
            title: Text('Some other setting'),
          ),
        ],
      )),
    );
  }
}
