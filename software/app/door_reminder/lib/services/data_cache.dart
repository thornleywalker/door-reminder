import 'package:door_reminder/objects/device.dart';
import 'package:door_reminder/services/notifications.dart';
import 'package:flutter/material.dart';

import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:door_reminder/services/authentication.dart';
import 'package:door_reminder/objects/reminder.dart';

class DataCache {
  static DataCache _dataCache;

  //DataCache definition
  DataCache._internal();
  factory DataCache() {
    if (_dataCache == null) {
      _dataCache = DataCache._internal();
    }
    return _dataCache;
  }

  //*****reminders*****
  List<Reminder> _reminderList = new List();

  void addReminder(Reminder reminder) {
    _reminderList.add(reminder);
    _addReminderToDatabase(reminder);
  }

  void removeReminder(Reminder reminder) {
    _reminderList.remove(reminder);
    _deleteReminderFromDatabase(reminder);
  }

  Future<bool> syncReminders() async {
    await _remindersFromDatabase();
    return true;
  }

  List<Reminder> getReminderList() => _reminderList;

  //*****devices*****
  List<Device> _deviceList = new List();

  void addDevice(Device device) {
    //add device to list
    _deviceList.add(device);
    // add device to database
    _addDeviceToDatabase(device);
    //subscribe to device notifications
    Notifications().subscribe(device.id);
  }

  void removeDevice(Device device) {
    _deviceList.remove(device);
    _removeDeviceFromDatabase(device);
  }

  Future<bool> syncDevices() async {
    return true;
  }

  List<Device> getDeviceList() => _deviceList;

  ////////////////////////////////////////////////////
  /////////// Firebase ///////////////////////////////
  ////////////////////////////////////////////////////

  //*****database*****
  final FirebaseDatabase _database = FirebaseDatabase.instance;

  //get get user reminders from database
  Future<void> _remindersFromDatabase() async {
    DatabaseReference reminderGet = _database
        .reference()
        .child('users')
        .child(await userID())
        .child('reminders');

    await reminderGet.once().then((snap) {
      Map<dynamic, dynamic> remindersMap = snap.value;
      _reminderList = List();

      remindersMap.forEach((key, value) {
        Reminder reminder = Reminder(
            key: key,
            description: value['description'],
            body: value['body'],
            destination: value['destination'],
            direction: value['direction'],
            uid: value['user-id'],
            did: value['device-id']);
        _reminderList.add(reminder);
      });
    });
  }

  //add reminder to database
  Future<void> _addReminderToDatabase(Reminder reminder) async {
    //add to users
    DatabaseReference userPush = _database
        .reference()
        .child('users')
        .child(reminder.uid)
        .child('reminders')
        .push();
    userPush.set(reminder.toJson());
    //feedback reminder key
    reminder.key = userPush.key;

    //increment device reminder count
    var dbCount = _database
        .reference()
        .child('devices')
        .child(reminder.did)
        .child(reminder.uid)
        .child(reminder.direction.toLowerCase())
        .child('count');

    var currCount = dbCount.once();
    currCount.then((snap) {
      if (snap.value == null)
        dbCount.set(1);
      else
        dbCount.set(snap.value + 1);
    });
  }

  //add reminder to database
  Future<void> _deleteReminderFromDatabase(Reminder reminder) async {
    //add to users
    DatabaseReference userPush = _database
        .reference()
        .child('users')
        .child(reminder.uid)
        .child('reminders')
        .child(reminder.key);
    userPush.set(null);

    //increment device reminder count
    var dbCount = _database
        .reference()
        .child('devices')
        .child(reminder.did)
        .child(reminder.uid)
        .child(reminder.direction.toLowerCase())
        .child('count');

    var currCount = dbCount.once();
    currCount.then((snap) {
      if (snap.value > 0)
        dbCount.set(snap.value - 1);
      else
        dbCount.set(0);
    });
  }

  Future<void> _addDeviceToDatabase(Device device) async {
    String deviceID = device.id;
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

  Future<void> _removeDeviceFromDatabase(Device device) async {}

  //*****user information*****
  String _userID;
  bool _userIDSet = false;
  String _email;
  bool _emailSet = false;
  String _username;
  bool _usernameSet = false;

  void verifyInfo() {
    var currUser = FirebaseAuth.instance.currentUser;
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

  String getUserID() => this._userID;

  Future<String> username() async {
    if (!_usernameSet) await verifyInfo();
    return _username;
  }

  Future<String> email() async {
    if (!_emailSet) await verifyInfo();
    return _email;
  }

  Future<String> userID() async {
    if (!_userIDSet) verifyInfo();
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
    clearDataCache();
    return logoutCallback;
  }

  void clearDataCache() {
    deleteEmail();
    deleteUserID();
    deleteUsername();
  }
}
