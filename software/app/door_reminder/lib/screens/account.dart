//import 'dart:html';

import 'package:flutter/material.dart';
import 'package:door_reminder/widgets/hamburger_menu.dart';
import 'package:door_reminder/services/data_cache.dart';

class AccountRoute extends StatelessWidget {
  final dc = DataCache();
  var context;

  signOut() async {
    while (Navigator.canPop(context)) {
      Navigator.pop(context);
    }

    try {
      await dc.auth.signOut();
      dc.logoutCallback();
    } catch (e) {
      print(e);
    }
    dc.clearDataCache();
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
                  future: dc.userID(),
                  builder: (context, AsyncSnapshot<String> uid) {
                    if (uid.hasData)
                      return Text('UserID: ' + uid.data);
                    else
                      return Text('Loading');
                  }),
              FutureBuilder(
                  future: dc.email(),
                  builder: (context, AsyncSnapshot<String> email) {
                    if (email.hasData)
                      return Text('email: ' + email.data);
                    else
                      return Text('Loading');
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
