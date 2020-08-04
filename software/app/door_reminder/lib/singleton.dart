import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:door_reminder/login_services/authentication.dart';
import 'package:door_reminder/testing.dart';

class Singleton {
  //Singleton definition
  static final Singleton _singleton = Singleton.internal();
  Singleton.internal();
  factory Singleton() => _singleton;

  //database
  final FirebaseDatabase _database = FirebaseDatabase.instance;
  final FirebaseDatabase _db2 = FirebaseDatabase(app: FirebaseApp.instance);

  String deviceID = 'test-device-id';

  Future<String> testMethod() async {
    var _todoQuery = _db2.reference().child("devices").child(deviceID);
    Todo testTodo = Todo(
      subject: 'subject',
      userId: 'uid',
      completed: false,
    );
    _todoQuery.push().set(testTodo.toJson());

    return 'tried to add to database';
  }

  //login/logout
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

  //user information
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
}
