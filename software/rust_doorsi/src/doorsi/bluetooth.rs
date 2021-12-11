use std::{
    collections::HashMap,
    sync::{Arc, Mutex}
};
use esp_idf_svc::nvs;

use super::user::{User, UserId};

pub type BluetoothPower = u16;
type BluetoothInfo = u8;

pub struct BluetoothManager {
    user_levels: Vec<Arc<Mutex<BluetoothPower>>>,
    user_bt_info: HashMap<UserId, BluetoothInfo>,
}
impl BluetoothManager {
    pub fn new(users: &Vec<User>) -> Self {
        let mut power_vec = vec![];
        for usr in users {
            power_vec.push(usr.curr_power.clone());
        }

        let mut bt_info: HashMap<UserId, BluetoothInfo> = HashMap::new();
        // get bt infos from nvm

        BluetoothManager {
            user_levels: power_vec,
            user_bt_info: bt_info,
        }
    }
}