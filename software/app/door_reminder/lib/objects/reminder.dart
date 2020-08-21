import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:door_reminder/screens/reminder_change.dart';

import 'package:door_reminder/singleton.dart';

class ReminderListTile extends StatelessWidget {
  ReminderListTile({this.reminder});
  final Reminder reminder;

  @override
  Widget build(BuildContext context) {
    return ListTile(
      leading: Icon(Icons.ac_unit),
      title: Text(reminder.body != null ? reminder.body : 'none'),
      subtitle:
          Text(reminder.destination != null ? reminder.destination : 'none'),
      trailing: Container(
        width: 10,
        child: FlatButton(
            child: Icon(Icons.more_vert),
            onPressed: () {
              Navigator.push(
                  context,
                  MaterialPageRoute(
                    builder: (context) => ReminderEditor(reminder: reminder),
                  ));
            }),
      ),
    );
  }
}

class ReminderForm extends StatefulWidget {
  ReminderForm({Reminder reminder}) {
    if (reminder == null) {
      this.reminder = Reminder();
      this.editing = false;
    } else {
      this.reminder = reminder;
      this.editing = true;
    }
  }
  Reminder reminder;
  bool editing;

  @override
  State<StatefulWidget> createState() => _ReminderFormState();
}

class _ReminderFormState extends State<ReminderForm> {
  final _formKey = GlobalKey<FormState>();
  var singleton = Singleton();
  var tempReminder = Reminder();

  String displayValue = 'device-user';

  @override
  Widget build(BuildContext context) {
    tempReminder = widget.reminder.clone();
    return Form(
      autovalidate: true,
      key: _formKey,
      child: Column(
        children: <Widget>[
          TextFormField(
            decoration: InputDecoration(
                labelText: 'Body', hintText: 'Bring box of books'),
            initialValue: tempReminder.body,
            validator: (value) {
              if (value.isEmpty) {
                return 'reminder body cannot be empty';
              }
              return null;
            },
            onChanged: (value) {
              tempReminder.body = value;
            },
          ),
          TextFormField(
            decoration: InputDecoration(
                labelText: 'Destination', hintText: '(optional)'),
            initialValue: tempReminder.destination,
            onChanged: (value) {
              tempReminder.destination = value;
            },
          ),
          DropdownButtonFormField(
              value: displayValue,
              items: singleton
                  .getDeviceUsers()
                  .map<DropdownMenuItem<String>>((String value) {
                return DropdownMenuItem<String>(
                    value: value, child: Text(value));
              }).toList(),
              onChanged: (value) {
                setState(() {
                  displayValue = value;
                });
              }),
          Row(mainAxisAlignment: MainAxisAlignment.center, children: <Widget>[
            if (widget.editing)
              Padding(
                padding: const EdgeInsets.all(10),
                child: RaisedButton(
                  onPressed: () {
                    Navigator.pop(context);
                  },
                  child: Text('Cancel'),
                ),
              ),
            Padding(
              padding: const EdgeInsets.all(10),
              child: RaisedButton(
                onPressed: () {
                  if (_formKey.currentState.validate()) {
                    if (!widget.editing)
                      singleton.addReminder(tempReminder);
                    else
                      widget.reminder = tempReminder;
                    Navigator.pop(context);
                  }
                },
                child: Text(widget.editing ? 'Save' : 'Create'),
              ),
            ),
          ])
        ],
      ),
    );
  }
}

class Reminder {
  Reminder({this.body, this.destination, this.direction, this.uid});

  String key;
  @required
  String body = '';
  String destination = '';
  String direction = '';
  String uid = '';

  Reminder.fromSnapshot(String key, Map<dynamic, dynamic> snapshot)
      : key = key,
        body = snapshot['body'],
        destination = snapshot['destination'],
        direction = snapshot['direction'],
        uid = snapshot['user-id'];

  toJson() {
    return {
      'body': body,
      'destination': destination,
      'direction': direction,
      'user-id': uid
    };
  }

  Reminder clone() {
    var returnReminder = Reminder(
      body: this.body,
      destination: this.destination,
      direction: this.direction,
      uid: this.uid,
    );

    return returnReminder;
  }
}
