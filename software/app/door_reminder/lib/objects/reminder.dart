import 'package:firebase_database/firebase_database.dart';

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
