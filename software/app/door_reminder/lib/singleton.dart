import 'package:flutter/material.dart';

import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:door_reminder/login_services/authentication.dart';
import 'package:door_reminder/objects/reminder.dart';

class Singleton {
  //Singleton definition
  Singleton.internal();
  static final Singleton _singleton = Singleton.internal();
  factory Singleton() => _singleton;

  //*****reminders*****
  List<Reminder> _reminderList = new List();
  void addReminder(Reminder reminder) {
    _reminderToDatabase(reminder);
    _reminderList.add(reminder);
  }

  //*****devices*****
  void addDevice(String deviceID) {
    _deviceToDatabase(deviceID);
  }

  //*****firebase*****

  //*****database*****
  final FirebaseDatabase _database = FirebaseDatabase.instance;

  String _deviceID = 'new-device';

  //add reminder to database
  Future<void> _reminderToDatabase(Reminder reminder) async {
    //push new reminder to device reminders
    DatabaseReference newPush = _database
        .reference()
        .child('devices')
        .child(_deviceID)
        .child('reminders')
        .push();
    newPush.set(reminder.toJson());

    //add key to reminder for later deletion
    reminder.key = newPush.key;

    //increment count
    var currCount = await _database
        .reference()
        .child('devices')
        .child(_deviceID)
        .child('users')
        .child(await userID())
        .child('reminder-count')
        .once();

    if (currCount.value != null) {
      _database
          .reference()
          .child('devices')
          .child(_deviceID)
          .child('users')
          .child(await userID())
          .child('reminder-count')
          .set(currCount.value + 1);
    } else {
      _database
          .reference()
          .child('devices')
          .child(_deviceID)
          .child('users')
          .child(await userID())
          .child('reminder-count')
          .set(1);
    }

    //add reminder key to user's reminders
    _database
        .reference()
        .child('devices')
        .child(_deviceID)
        .child('users')
        .child(await userID())
        .child('reminders')
        .push()
        .set(newPush.key);
  }

  Future<void> _deviceToDatabase(String deviceID) async {
    //add device to user's devices
    _database
        .reference()
        .child('users')
        .child(await userID())
        .child('devices')
        .child(deviceID)
        .set(0);

    //check for device user
    var deviceUser = await _database
        .reference()
        .child('devices')
        .child(deviceID)
        .child('users')
        .child('device-user')
        .child('reminder-count')
        .once();

    if (deviceUser.value == null) {
      _database
          .reference()
          .child('devices')
          .child(deviceID)
          .child('users')
          .child('device-user')
          .set({'reminder-count': 0});
    }

    //add user to device users, set initial reminder count to 0
    _database
        .reference()
        .child('devices')
        .child(deviceID)
        .child('users')
        .child(await userID())
        .set({'reminder-count': 0});
  }
  //add device to user

  //*****user information*****
  String _userID;
  bool _userIDSet = false;
  String _email;
  bool _emailSet = false;
  String _username;
  bool _usernameSet = false;

  Future<void> verifyInfo() async {
    var currUser = await FirebaseAuth.instance.currentUser();
    setEmail(currUser.email);
    setUserID(currUser.uid);
    setUsername(currUser.displayName);
  }

  void setEmail(String email) {
    this._email = email;
    this._emailSet = true;
  }

  void setUserID(String uid) {
    this._userID = uid;
    this._userIDSet = true;
  }

  void setUsername(String username) {
    this._username = username;
    this._usernameSet = true;
  }

  Future<String> username() async {
    if (!_usernameSet) await verifyInfo();
    return _username;
  }

  Future<String> email() async {
    if (!_emailSet) await verifyInfo();
    return _email;
  }

  Future<String> userID() async {
    if (!_userIDSet) await verifyInfo();
    return _userID;
  }

  void deleteEmail() {
    _email = '';
    _emailSet = false;
  }

  void deleteUserID() {
    _userID = '';
    _userIDSet = false;
  }

  void deleteUsername() {
    _username = '';
    _usernameSet = false;
  }

  //*****login/logout*****
  BaseAuth auth;
  VoidCallback logoutCallback;

  void setAuth(BaseAuth auth) {
    this.auth = auth;
  }

  void setLogoutCallback(VoidCallback logoutCallback) {
    this.logoutCallback = logoutCallback;
  }

  BaseAuth getAuth() => auth;
  VoidCallback getLogoutCallback() {
    clearSingleton();
    return logoutCallback;
  }

  void clearSingleton() {
    deleteEmail();
    deleteUserID();
    deleteUsername();
  }
}
