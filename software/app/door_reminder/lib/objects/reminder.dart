import 'package:firebase_database/firebase_database.dart';

class Reminder {
  Reminder(
      {this.description,
      this.body,
      this.destination,
      this.direction,
      this.uid,
      this.did});

  String key;
  String description;
  String body;
  String destination;
  String direction;
  String uid;
  String did;

  Reminder.fromSnapshot(DataSnapshot snapshot)
      : key = snapshot.key,
        description = snapshot.value['description'],
        body = snapshot.value['body'],
        destination = snapshot.value['destination'],
        direction = snapshot.value['direction'],
        uid = snapshot.value['user-id'],
        did = snapshot.value['device-id'];

  toJson() {
    return {
      'description': description,
      'body': body,
      'destination': destination,
      'direction': direction,
      'user-id': uid,
      'device-id': did
    };
  }
}
