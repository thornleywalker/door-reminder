import 'package:door_reminder/objects/reminder.dart';
import 'package:door_reminder/services/data_cache.dart';
import 'package:flutter/material.dart';

class ReminderBuilder extends StatefulWidget {
  @override
  State<StatefulWidget> createState() {
    return _ReminderBuilder();
  }
}

class _ReminderBuilder extends State<ReminderBuilder> {
  Reminder reminder = Reminder();

  String description, body, destination;
  bool direction;

  final formKey = GlobalKey<FormState>();

  @override
  Widget build(BuildContext context) {
    return AlertDialog(
        contentPadding: EdgeInsets.all(10),
        content: SingleChildScrollView(
          child: Column(
            children: [
              Form(
                key: formKey,
                child: Column(
                  children: [
                    TextFormField(
                      decoration: InputDecoration(
                          hintText: "Description", helperText: "Description"),
                      validator: (value) {
                        if (value.isEmpty)
                          return "Description can't be left empty";
                        return null;
                      },
                      onSaved: (val) => reminder.description = val,
                    ),
                    TextFormField(
                      keyboardType: TextInputType.multiline,
                      maxLines: null,
                      decoration:
                          InputDecoration(hintText: "Body", helperText: "Body"),
                      validator: (value) {
                        return null;
                      },
                      onSaved: (val) => reminder.body = val,
                    ),
                    TextFormField(
                      decoration: InputDecoration(
                          hintText: "Destination", helperText: "Destination"),
                      validator: (value) {
                        return null;
                      },
                      onSaved: (val) => reminder.description = val,
                    ),
                    DropdownButtonFormField<String>(
                      decoration: InputDecoration(
                          hintText: "Select direction",
                          helperText: "Direction"),
                      items: ["Going", "Coming"].map((val) {
                        return DropdownMenuItem(
                          value: val,
                          child: Text(val),
                        );
                      }).toList(),
                      onChanged: (val) =>
                          reminder.direction = val.toLowerCase(),
                      validator: (val) {
                        if (reminder.direction == null)
                          return "Direction required";
                        return null;
                      },
                      onSaved: (val) => reminder.direction = val.toLowerCase(),
                    )
                  ],
                ),
              ),
              Row(
                children: [
                  FlatButton(
                    onPressed: () {
                      Navigator.pop(context);
                    },
                    child: Text("Cancel"),
                  ),
                  ElevatedButton(
                      onPressed: () {
                        if (formKey.currentState.validate()) {
                          print("form is valid");
                          formKey.currentState.save();
                          reminder.uid = 'testid';
                          reminder.did = 'testdevice';
                          DataCache().addReminder(reminder);
                          Navigator.pop(context);
                        } else
                          print("form invalid");
                      },
                      child: Text("Create"))
                ],
              ),
            ],
          ),
        ));
  }
}
