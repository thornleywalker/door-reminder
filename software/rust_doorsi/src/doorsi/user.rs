use super::{reminder::Reminder, bluetooth::BluetoothPower};
use std::sync::{Arc, Mutex};

pub type UserId = String;

pub struct User {
    pub id: String,
    pub reminders: Vec<Reminder>,
    pub threshold: BluetoothPower,
    pub curr_power: Arc<Mutex<BluetoothPower>>,
}
impl User {
    pub fn new(id: String) -> Self {
        User {
            id: id,
            reminders: vec![],
            threshold: 15,
            curr_power: Arc::new(Mutex::new(0)),
        }
    }
}