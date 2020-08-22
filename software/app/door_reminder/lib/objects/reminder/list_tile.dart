import 'package:flutter/material.dart';

import 'package:door_reminder/objects/reminder.dart';
import 'package:door_reminder/objects/reminder/reminder_change.dart';
import 'package:door_reminder/singleton.dart';

class ReminderListTile extends StatefulWidget {
  ReminderListTile({@required this.notifyParent, this.reminder});
  final Reminder reminder;
  final Function() notifyParent;

  State<ReminderListTile> createState() => _ReminderListTileState();
}

class _ReminderListTileState extends State<ReminderListTile> {
  bool choicesOn = false;
  var singleton = Singleton();
  @override
  Widget build(BuildContext context) {
    return Column(children: <Widget>[
      FlatButton(
        padding: const EdgeInsets.all(0),
        visualDensity: VisualDensity(
            vertical: VisualDensity.minimumDensity,
            horizontal: VisualDensity.minimumDensity),
        child: ListTile(
          contentPadding: const EdgeInsets.all(0),
          leading: Icon(
            widget.reminder.direction == 'going'
                ? Icons.arrow_right
                : Icons.arrow_left,
          ),
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
              onPressed: (() {
                singleton.deleteReminder(widget.reminder);
                widget.notifyParent();
              }),
            )
          ])),
      Divider(
        color: Colors.black,
      ),
    ]);
  }
}
