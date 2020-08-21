import 'dart:async';
import 'dart:io';

import 'package:firebase_messaging/firebase_messaging.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';

import 'package:door_reminder/singleton.dart';
import 'package:door_reminder/login_services/authentication.dart';
import 'package:door_reminder/hamburger_menu.dart';
import 'package:door_reminder/account_menu.dart';
import 'package:door_reminder/objects/reminder.dart';

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

  final FirebaseMessaging _fcm = FirebaseMessaging();
  StreamSubscription iosSubscription;

  @override
  void initState() {
    super.initState();

    singleton.initialize();

    //setup push messaging from FCM
    if (Platform.isIOS) {
      iosSubscription = _fcm.onIosSettingsRegistered.listen((data) {
        // save the token  OR subscribe to a topic here
      });

      _fcm.requestNotificationPermissions(IosNotificationSettings());
    }

    _fcm.configure(
      onMessage: (Map<String, dynamic> message) async {
        print("onMessage: $message");
      },
      onLaunch: (Map<String, dynamic> message) async {
        print("onLaunch: $message");
      },
      onResume: (Map<String, dynamic> message) async {
        print("onResume: $message");
      },
    );
  }

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
      body: Container(
          height: MediaQuery.of(context).size.height,
          width: MediaQuery.of(context).size.width,
          child: FutureBuilder(
              future: singleton.getReminderList(),
              builder: (context, AsyncSnapshot<List<Reminder>> list) {
                return ListView(
                  padding: const EdgeInsets.all(8),
                  children: list.data.map<Widget>((Reminder value) {
                    return ReminderListTile(reminder: value);
                  }).toList(),
                );
              })),
      floatingActionButton: FloatingActionButton(
        child: Icon(Icons.add),
        onPressed: () async {
          await showDialog<String>(
              context: context,
              builder: (BuildContext context) {
                return AlertDialog(
                  title: Text("Create new Reminder"),
                  content: Container(
                    child: ReminderForm(),
                  ),
                );
              });
          setState(() {});
        },
      ),
    );
  }
}
