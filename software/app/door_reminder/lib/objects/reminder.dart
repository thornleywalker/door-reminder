import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';

import 'package:door_reminder/singleton.dart';

class ReminderForm extends StatefulWidget {
  @override
  State<StatefulWidget> createState() => _ReminderFormState();
}

class _ReminderFormState extends State<ReminderForm> {
  final _formKey = GlobalKey<FormState>();
  var singleton = Singleton();
  var newReminder = Reminder();

  @override
  Widget build(BuildContext context) {
    return Form(
      autovalidate: true,
      key: _formKey,
      child: Column(
        children: <Widget>[
          TextFormField(
            decoration: InputDecoration(
                labelText: 'Body', hintText: 'Bring box of books'),
            validator: (value) {
              if (value.isEmpty) {
                return 'please enter some text';
              }
              return null;
            },
            onChanged: (value) {
              newReminder.body = value;
            },
          ),
          TextFormField(
            decoration: InputDecoration(
                labelText: 'Destination', hintText: '(optional)'),
            onChanged: (value) {
              newReminder.destination = value;
            },
          ),
          RaisedButton(
            onPressed: () {
              // Validate returns true if the form is valid, or false
              // otherwise.
              if (_formKey.currentState.validate()) {
                singleton.addReminder(newReminder);
                Navigator.pop(context);
              }
            },
            child: Text('Create'),
          ),
        ],
      ),
    );
  }
}

class Reminder {
  Reminder({this.body, this.destination, this.direction, this.uid});

  String key;
  String body;
  String destination;
  String direction;
  String uid;

  Reminder.fromSnapshot(DataSnapshot snapshot)
      : key = snapshot.key,
        body = snapshot.value['body'],
        destination = snapshot.value['destination'],
        direction = snapshot.value['direction'],
        uid = snapshot.value['user-id'];

  toJson() {
    return {
      'body': body,
      'destination': destination,
      'direction': direction,
      'user-id': uid
    };
  }
}
