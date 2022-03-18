use super::{bluetooth::BluetoothPower, reminder::Reminder};
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
    pub fn from_db() -> Self {
        panic!("from db not implemented")
    }
    pub fn trigger(&mut self) {
        if curr_power > threshold {
            for reminder in reminders {
                reminder.trigger();
            }
        }
    }
}
