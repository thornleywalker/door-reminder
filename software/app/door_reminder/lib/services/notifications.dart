import 'dart:async';

import 'package:firebase_messaging/firebase_messaging.dart';
import 'package:flutter/material.dart';

class Notifications {
  Notifications(BuildContext context) {
    fbMess.requestNotificationPermissions(); //asks permission for iOS

    fbMess.configure(onBackgroundMessage: (Map<String, dynamic> message) async {
      showDialog<bool>(
              context: context, builder: (_) => Text("this is a message"))
          .then((bool shouldNavigate) {
        if (shouldNavigate == true) {}
      });
    });

    fbMess.requestNotificationPermissions(const IosNotificationSettings(
        sound: true, badge: true, alert: true, provisional: true));
    fbMess.onIosSettingsRegistered.listen((IosNotificationSettings settings) {
      print("Settings registered: $settings");
    });
    fbMess.getToken().then((String token) {
      if (token != null) {}
    });
  }

  static final fbMess = FirebaseMessaging();
}
