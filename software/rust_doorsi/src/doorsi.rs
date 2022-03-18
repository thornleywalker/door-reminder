#[cfg(test)]
mod tests;

mod bluetooth;
mod db_proxy;
mod ir_sensor;
mod reminder;
mod user;

use chrono::{Date, DateTime, Timelike, Utc};
use esp_idf_hal::{gpio::*, i2c, peripherals::Peripherals};
use esp_idf_sys::*;
use log::*;
use std::collections::HashMap;

use bluetooth::*;
use db_proxy::*;
use ir_sensor::*;
use reminder::Reminder;
use user::*;

pub struct Doorsi {
    pub users: HashMap<String, User>,
    active_reminders: Vec<Reminder>,
    //ir_manager: IrManager,
    db_proxy: DbProxy,
    bluetooth_manager: BluetoothManager,
}
impl Doorsi {
    pub fn new() -> Result<Self, EspError> {
        info!("Initializing Doorsi");
        let peripherals = Peripherals::take().unwrap();
        let pins = peripherals.pins;
        info!("Peripherals acquired");

        // db proxy
        let mut db_proxy = DbProxy {};

        // users
        let mut users = vec![];
        users.push(User::new("default_id".to_string()));
        info!("Users created");

        // bt manager
        let mut bt_man = BluetoothManager::new(&users);
        info!("Bluetooth manager created");

        Ok(Doorsi {
            users: HashMap::new(),
            active_reminders: vec![],
            //ir_manager: IrManager::new(peripherals.i2c0, pins.gpio4, pins.gpio16)?,  // check that these are the pins
            db_proxy: db_proxy,
            bluetooth_manager: bt_man,
        })
    }
    pub fn run(self) -> ! {
        info!("Beginning Doorsi");
        loop {}
    }
    // general trigger function. Can occur on button press, door contact, ir sensor, etc.
    pub fn trigger(&mut self) {
        // get all active reminders
        // for each active reminder, send a notification
    }
    pub fn activate_reminders(
        &self,
        user_power_levels: Vec<(UserId, BluetoothPower)>,
    ) -> Vec<Reminder> {
        let mut ret_vec = vec![];
        for (user_id, power) in user_power_levels {
            let temp_user = &self.users[&user_id];
            for reminder in &self.active_reminders {
                if (reminder.user_id == user_id) && (power > temp_user.threshold) {
                    ret_vec.push(reminder.clone());
                }
            }
        }
        ret_vec
    }

    pub fn update_active_list(&mut self, now: DateTime<Utc>) {
        // remove non active reminders
        for i in 0..self.active_reminders.len() {
            if self.active_reminders[i].is_active(&now) {
                self.active_reminders.remove(i);
            }
        }

        // add active reminders
        for user in self.users.values() {
            for reminder in &user.reminders {
                if reminder.is_active(&now) {
                    self.active_reminders.push(reminder.clone());
                }
            }
        }
    }

    pub fn add_user(&mut self, user: User) {
        self.users.insert(user.id.to_string(), user);
    }
    pub fn get_users(&self) -> Vec<String> {
        vec!["".to_string()]
    }
}
