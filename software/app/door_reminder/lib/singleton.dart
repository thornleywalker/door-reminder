import 'package:flutter/material.dart';
import 'package:door_reminder/login_services/authentication.dart';

class Singleton {
  //Singleton definition
  static final Singleton _singleton = Singleton.internal();
  Singleton.internal();
  factory Singleton() => _singleton;

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

  void clearSingleton() {}
}
