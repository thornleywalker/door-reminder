import 'package:flutter/material.dart';
import 'package:door_reminder/screens/account.dart';
import 'package:door_reminder/screens/settings.dart';
import 'package:door_reminder/screens/help.dart';
import 'package:door_reminder/singleton.dart';

class HamburgerMenu extends StatelessWidget {
  HamburgerMenu();

  static var mySingleton = Singleton();
  static String userName, userEmail;

  @override
  Widget build(BuildContext context) {
    return Drawer(
      child: ListView(children: <Widget>[
        UserAccountsDrawerHeader(
          accountName: FutureBuilder(
              future: mySingleton.userID(),
              builder: (context, AsyncSnapshot<String> uid) {
                if (uid.hasData)
                  return Text('UserID: ' + uid.data);
                else
                  return Text('Loading...');
              }),
          accountEmail: FutureBuilder(
              future: mySingleton.email(),
              builder: (context, AsyncSnapshot<String> email) {
                if (email.hasData)
                  return Text('UserID: ' + email.data);
                else
                  return Text('Loading...');
              }),
          currentAccountPicture: CircleAvatar(
            backgroundColor: Theme.of(context).platform == TargetPlatform.iOS
                ? Colors.blue
                : Colors.white,
            child: FutureBuilder(
                future: mySingleton.email(),
                builder: (context, AsyncSnapshot<String> email) {
                  if (email.hasData)
                    return Text(
                      email.data.substring(0, 1).toUpperCase(),
                      style: TextStyle(fontSize: 40.0),
                    );
                  else
                    return Text('Loading...');
                }),
          ),
        ),
        ListTile(
          title: Text("Home"),
          onTap: () {
            while (Navigator.canPop(context)) {
              Navigator.pop(context);
            }
          },
        ),
        ListTile(
          title: Text("Account"),
          onTap: () {
            Navigator.pop(context); //close drawer
            Navigator.push(
              context,
              MaterialPageRoute(builder: (context) => AccountRoute()),
            );
          },
        ),
        ListTile(
          title: Text("Settings"),
          onTap: () {
            Navigator.pop(context); //close drawer
            Navigator.push(
              context,
              MaterialPageRoute(builder: (context) => SettingsRoute()),
            );
          },
        ),
        ListTile(
          title: Text("Help"),
          onTap: () {
            Navigator.pop(context); //close drawer
            Navigator.push(
              context,
              MaterialPageRoute(builder: (context) => HelpRoute()),
            );
          },
        ),
      ]),
    );
  }
}
