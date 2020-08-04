//import 'dart:html';

import 'package:flutter/material.dart';
import 'package:door_reminder/hamburger_menu.dart';
import 'package:door_reminder/singleton.dart';

class AccountRoute extends StatelessWidget {
  var mySingleton = Singleton();
  var context;

  signOut() async {
    while (Navigator.canPop(context)) {
      Navigator.pop(context);
    }

    try {
      await mySingleton.auth.signOut();
      mySingleton.logoutCallback();
    } catch (e) {
      print(e);
    }
    mySingleton.clearSingleton();
  }

  @override
  Widget build(BuildContext context) {
    this.context = context;

    return Scaffold(
      appBar: AppBar(
        title: Text('Account'),
      ),
      drawer: HamburgerMenu(),
      body: Center(
        child: Column(
            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
            children: <Widget>[
              //Text('Username: ' + mySingleton.username()),
              FutureBuilder(
                  future: mySingleton.userID(),
                  builder: (context, AsyncSnapshot<String> uid) {
                    if (uid.hasData) return Text('UserID: ' + uid.data);
                  }),
              FutureBuilder(
                  future: mySingleton.email(),
                  builder: (context, AsyncSnapshot<String> email) {
                    if (email.hasData) return Text('email: ' + email.data);
                  }),
              Text('Quest: To seek the holy grail'),
              Text('Favorite Color: Green'),
              RaisedButton(
                child: Text("Sign Out"),
                onPressed: signOut,
              ),
            ]),
      ),
    );
  }
}
