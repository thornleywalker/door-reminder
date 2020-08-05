import 'package:flutter/material.dart';
import 'package:door_reminder/screens/account.dart';
import 'package:door_reminder/singleton.dart';

class AccountMenu extends StatelessWidget {
  AccountMenu();
  static var mySingleton = Singleton();
  //var context;

  @override
  Widget build(BuildContext context) {
    //this.context = context;
    return IconButton(
        icon: Icon(Icons.account_circle),
        iconSize: 45,
        onPressed: () {
          Navigator.push(
            context,
            MaterialPageRoute(builder: (context) => AccountRoute()),
          );
        });
  }
}
