import 'package:firebase_database/firebase_database.dart';

class Device {
  Device({this.location, this.id});

  String key;
  String location;
  String id;

  Device.fromSnapshot(DataSnapshot snapshot)
      : key = snapshot.key,
        location = snapshot.value['location'],
        id = snapshot.value['device-id'];

  toJson() {
    return {'location': location, 'user-id': id};
  }
}
