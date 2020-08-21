import 'package:flutter/material.dart';
import 'package:door_reminder/objects/reminder.dart';
import 'package:door_reminder/objects/reminder/reminder_change.dart';

class ReminderListTile extends StatefulWidget {
  ReminderListTile({this.reminder});
  final Reminder reminder;

  State<ReminderListTile> createState() => _ReminderListTileState();
}

class _ReminderListTileState extends State<ReminderListTile> {
  bool choicesOn = false;
  @override
  Widget build(BuildContext context) {
    return Column(children: <Widget>[
      FlatButton(
        padding: const EdgeInsets.all(0),
        child: ListTile(
          title: Text(
              widget.reminder.body != null ? widget.reminder.body : 'none'),
          subtitle: Text(widget.reminder.destination != null
              ? widget.reminder.destination
              : 'none'),
        ),
        onPressed: () {
          Navigator.push(
              context,
              MaterialPageRoute(
                builder: (context) => ReminderEditor(reminder: widget.reminder),
              ));
        },
        onLongPress: () {
          choicesOn = !choicesOn;
          setState(() {});
        },
      ),
      //if (choicesOn)
      Container(
          height: choicesOn ? 35 : 0,
          color: Colors.grey[300],
          child: Row(mainAxisAlignment: MainAxisAlignment.end, children: [
            FlatButton(
              child: Text('edit'),
              color: Colors.grey[400],
              onPressed: (() {
                Navigator.push(
                    context,
                    MaterialPageRoute(
                      builder: (context) =>
                          ReminderEditor(reminder: widget.reminder),
                    ));
              }),
            ),
            FlatButton(
              padding: const EdgeInsets.all(0),
              child: Text('delete'),
              color: Colors.red,
              onPressed: (() {}),
            )
          ])),
      Divider(
        color: Colors.black,
      ),
    ]);
  }
}
