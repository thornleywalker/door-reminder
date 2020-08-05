import 'package:flutter/material.dart';
import 'package:door_reminder/login_services/login_page.dart';
import 'package:door_reminder/login_services/authentication.dart';

//is this necesary? isn't this the point of google-services?
// Future<void> main() async {
//   WidgetsFlutterBinding.ensureInitialized();
//   final FirebaseApp app = await FirebaseApp.configure(
//     name: 'db2',
//     options: Platform.isIOS
//         ? const FirebaseOptions(
//       googleAppID: '1:297855924061:ios:c6de2b69b03a5be8', //FIXME
//       gcmSenderID: '297855924061',  //FIXME
//       apiKey: 'AIzaSyBiRnnpJwOEa54zAvrVnJXdKxtnB9v_t44',
//       databaseURL: 'https://door-reminder-89e84.firebaseio.com',
//     )
//         : const FirebaseOptions(
//       googleAppID: '1:297855924061:android:669871c998cc21bd', //FIXME
//       apiKey: 'AIzaSyBXfZofsr_54pdEwMjgeJALrj0s5C4PGxA',
//       databaseURL: 'https://door-reminder-89e84.firebaseio.com',
//     ),
//   );
//   runApp(MyApp());
// }

void main() {
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
