import 'dart:async';
import 'dart:io';

import 'package:firebase_messaging/firebase_messaging.dart';

class Notifications {
  Notifications._internal();
  factory Notifications() {
    if (_notifications == null) _notifications = Notifications._internal();
    return _notifications;
  }
  static Notifications _notifications;

  final _fbMess = FirebaseMessaging();
  StreamSubscription iosSubscription;

  void init() {
    if (Platform.isIOS) {
      iosSubscription = _fbMess.onIosSettingsRegistered.listen((data) {
        // save the token  OR subscribe to a topic here
      });

      _fbMess.requestNotificationPermissions(IosNotificationSettings());
    }

    _fbMess.configure(
      onMessage: (Map<String, dynamic> message) async {
        print("onMessage: $message");
      },
      onLaunch: (Map<String, dynamic> message) async {
        print("onLaunch: $message");
      },
      onResume: (Map<String, dynamic> message) async {
        print("onResume: $message");
      },
    );
  }

  void subscribe(String topic) {
    _fbMess.subscribeToTopic(topic);
  }

  void unsubscribe(String topic) {
    _fbMess.unsubscribeFromTopic(topic);
  }
}
