import 'package:door_reminder/objects/reminder.dart';
import 'package:door_reminder/services/data_cache.dart';
import 'package:flutter/material.dart';

class ReminderBuilder extends StatefulWidget {
  ReminderBuilder({this.onComplete});

  Function onComplete;
  @override
  State<StatefulWidget> createState() {
    return _ReminderBuilder();
  }
}

class _ReminderBuilder extends State<ReminderBuilder> {
  DataCache dc = DataCache();

  TextEditingController desc = TextEditingController(),
      body = TextEditingController(),
      dest = TextEditingController(),
      dir = TextEditingController();

  // String description, body, destination;
  // bool direction;

  final formKey = GlobalKey<FormState>();

  @override
  Widget build(BuildContext context) {
    dir.text = "Going";
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
                    controller: desc,
                  ),
                  TextFormField(
                    keyboardType: TextInputType.multiline,
                    maxLines: null,
                    decoration:
                        InputDecoration(hintText: "Body", helperText: "Body"),
                    validator: (value) {
                      return null;
                    },
                    controller: body,
                  ),
                  TextFormField(
                    decoration: InputDecoration(
                        hintText: "Destination", helperText: "Destination"),
                    validator: (value) {
                      return null;
                    },
                    controller: dest,
                  ),
                  DropdownButtonFormField<String>(
                    decoration: InputDecoration(
                        hintText: "Select direction", helperText: "Direction"),
                    items: ["Going", "Coming"].map((val) {
                      return DropdownMenuItem(
                        value: val,
                        child: Text(val),
                      );
                    }).toList(),
                    onChanged: (val) => dir.text = val,
                    value: dir.text,
                    validator: (val) {
                      if (val.isEmpty) return "Direction required";
                      return null;
                    },
                    onSaved: (val) => dir.text = val.toLowerCase(),
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
                      dc.addReminder(
                        Reminder(
                            description: desc.text,
                            body: body.text,
                            destination: dest.text,
                            direction: dir.text,
                            uid: dc.getUserID(),
                            did: 'deviceid'),
                      );
                      Navigator.pop(context);
                      widget.onComplete();
                    } else
                      print("form invalid");
                  },
                  child: Text("Create"),
                )
              ],
            ),
          ],
        ),
      ),
    );
  }
}
