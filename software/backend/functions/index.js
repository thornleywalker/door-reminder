const functions = require('firebase-functions');
const admin = require('firebase-admin');
const { DataSnapshot } = require('firebase-functions/lib/providers/database');
admin.initializeApp();

// // Create and Deploy Your First Cloud Functions
// // https://firebase.google.com/docs/functions/write-firebase-functions
//
// exports.helloWorld = functions.https.onRequest((request, response) => {
//   functions.logger.info("Hello logs!", {structuredData: true});
//   response.send("Hello from Firebase!");
// });

/**
 * Triggers when one of a device's users has a pending reminder to be notified about
 * 
 * Flag is located at /devices/{deviceID}/{userID}/[coming, going]/flag
 */

exports.sendFollowerNotification = functions.database
  .ref('/devices/{deviceID}/{userID}/{direction}/flag')
    .onWrite(async (change, context) => {
    //Get user and device id from database reference
    const deviceID = context.params.deviceID;
    const userID = context.params.userID;
    const direction = context.params.direction;

    //check if the flag is lowered, ignore
    if (!change.after.val()) 
      return console.log('ignored call');

    // console.log('User', userID, 'has a', direction, 'reminder on device', deviceID);

    //User reminders reference
    const userRemindersReference = admin.database().ref(`/users/${userID}/reminders`);

    //get all reminders
    const getUserRemindersPromise = userRemindersReference.once('value');

    const lowerFlag = admin.database()
      .ref(`/devices/${deviceID}/${userID}/${direction}/flag`)
      .set(false);

    //wait for promise fullfilment
    const results = await Promise.all([getUserRemindersPromise, lowerFlag]);
    const remindersSnapshot = results[0];

    //create payoad variable to be set later
    let reminders = [];

    //go through each child of the snapshot
    remindersSnapshot.forEach((snap)=>{
      let reminderDirection = String(snap.child('direction').val());
      if(reminderDirection.toLowerCase() === direction){
        let newReminder = {
          title: String(snap.child('description').val()),
          body: String(snap.child('body').val())
        };
        reminders.push(newReminder);
      }
    });

    let numReminders = reminders.length;

    //create payload based on number of reminders
    let payload;
    if(numReminders <= 0){
      return console.log("Couldn't find any reminders");
    }
    else if(numReminders === 1){
      reminder = reminders[0];
      payload = {
        notification: {
          title: `${String(reminder.title)}`,
          body: `${String(reminder.body)}`
        }
      };
    }
    else{
      // console.log(`multi reminders`);
      payload = {
        notification: {
          title: `Multiple Reminder Notification for user ${userID}`,
          body: `Num reminders: ${numReminders}, Device ${deviceID}`
        }
      };
    }

    const response = await admin.messaging().sendToTopic(userID, payload);
    // console.log("notification sent");
    
    //lower flag

    return true;
  });