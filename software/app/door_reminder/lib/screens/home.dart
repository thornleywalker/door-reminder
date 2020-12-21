import 'package:door_reminder/services/notifications.dart';
import 'package:door_reminder/widgets/reminder_builder.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';

import 'package:door_reminder/services/data_cache.dart';
import 'package:door_reminder/services/authentication.dart';
import 'package:door_reminder/widgets/hamburger_menu.dart';
import 'package:door_reminder/widgets/account_menu.dart';

class HomePage extends StatefulWidget {
  HomePage({BaseAuth auth, VoidCallback logoutCallback}) {
    dc.setAuth(auth);
    dc.setLogoutCallback(logoutCallback);
    dc.syncReminders();
    notif.init();
    dc.userID().then((val) => notif.subscribe(val.toString()));
    dc.syncDevices().then((val) {
      if (val)
        dc.getDeviceList().forEach((device) {
          notif.subscribe(device.id);
        });
    });
  }

  final dc = DataCache();
  final notif = Notifications();

  final Map<String, bool> showMenu = Map();

  @override
  State<StatefulWidget> createState() => new _HomePageState();
}

class _HomePageState extends State<HomePage> {
  @override
  void initState() {
    super.initState();
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
      body: Column(
        children: <Widget>[
          Container(
            padding: EdgeInsets.fromLTRB(20, 10, 10, 0),
            alignment: Alignment.centerLeft,
            child: Text(
              'Reminders',
              style: TextStyle(fontSize: 24),
            ),
          ),
          Divider(),
          Expanded(
            child: FutureBuilder(
              future: widget.dc.syncReminders(),
              builder: (BuildContext context, AsyncSnapshot<dynamic> snapshot) {
                if (snapshot.hasData)
                  return ListView(
                    children: widget.dc.getReminderList().map((item) {
                      if (!widget.showMenu.containsKey(item.key))
                        widget.showMenu[item.key] = false;

                      return Column(
                        children: [
                          ListTile(
                            title: Text(item.description),
                            subtitle: Text(item.destination),
                            leading: Icon(
                                (item.direction.toLowerCase() == 'coming')
                                    ? Icons.arrow_back
                                    : Icons.arrow_forward),
                            trailing: IconButton(
                              icon: Icon(Icons.settings),
                              onPressed: () {
                                widget.showMenu[item.key] =
                                    !widget.showMenu[item.key];
                                setState(() {});
                              },
                            ),
                            onTap: () => print(item.description),
                          ),
                          Container(
                              height: (widget.showMenu[item.key]) ? 50 : 0,
                              child: ButtonBar(
                                buttonPadding:
                                    EdgeInsets.fromLTRB(10, 0, 10, 0),
                                children: [
                                  FlatButton(
                                    onPressed: () {
                                      widget.dc.removeReminder(item);
                                      setState(() {});
                                    },
                                    child: Text("Delete"),
                                    color: Colors.red,
                                  ),
                                  ElevatedButton(
                                    onPressed: () {},
                                    child: Text("Edit"),
                                  )
                                ],
                              ))
                        ],
                      );
                    }).toList(),
                  );
                else
                  return Text("loading");
              },
            ),
          ),
        ],
      ),
      floatingActionButton: FloatingActionButton(
        child: Icon(Icons.add),
        onPressed: () {
          showDialog(
            context: context,
            builder: (BuildContext context) {
              return ReminderBuilder(onComplete: () {
                setState(() {});
              });
            },
          );
        },
      ),
    );
  }
}
