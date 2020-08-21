import 'package:flutter/material.dart';
import 'package:door_reminder/objects/reminder.dart';

class ReminderEditor extends StatefulWidget {
  ReminderEditor({this.reminder});
  final Reminder reminder;

  @override
  State<ReminderEditor> createState() => _ReminderEditorState();
}

class _ReminderEditorState extends State<ReminderEditor> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
        appBar: AppBar(
          title: Text('Reminder Editor'),
        ),
        body: ReminderForm(reminder: widget.reminder));
  }
}
