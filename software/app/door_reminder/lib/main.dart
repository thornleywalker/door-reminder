import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:door_reminder/screens/login_page.dart';
import 'package:door_reminder/services/authentication.dart';

Future<void> main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp();

  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  MyApp();

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
        title: 'Doorsi',
        debugShowCheckedModeBanner: false,
        theme: new ThemeData(
          primarySwatch: Colors.blue,
        ),
        home: new LoginPage(auth: new Auth()));
  }
}
