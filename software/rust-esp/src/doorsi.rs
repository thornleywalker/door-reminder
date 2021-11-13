use std::collections::HashMap;

struct Doorsi {
    users: HashMap<String, User>,
    active_reminders: Vec<Reminder>,
}
type BluetoothPower = u16;

pub struct User {
    id: String,
    reminders: Vec<Reminder>,
    threshold: BluetoothPower,
}
impl User {
    pub fn new() -> Self {
        Reminder {

        }
    }
}

struct Reminder {

}

