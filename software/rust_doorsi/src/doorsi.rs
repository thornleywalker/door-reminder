#[cfg(test)]
mod tests;

use std::collections::HashMap;
// use chrono::{DateTime, Date, Utc};

type UserId = String;
type Hour = u8;

struct Doorsi {
    users: HashMap<String, User>,
    active_reminders: Vec<Reminder>,
}
impl Doorsi {
    pub fn new() -> Self {
        Doorsi {
            users: HashMap::new(),
            active_reminders: vec![],
        }
    }

    pub fn activate_reminders(&self, user_power_levels: Vec<(UserId, BluetoothPower)>) -> Vec<Reminder> {
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

    pub fn update_active_list(&mut self, now: DateTime<Utc>)
    {
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
}

type BluetoothPower = u16;

pub struct User {
    id: String,
    reminders: Vec<Reminder>,
    threshold: BluetoothPower,
}
impl User {
    pub fn new(id: String) -> Self {
        User {
            id: id,
            reminders: vec![],
            threshold: 15,
        }
    }
}

#[derive(Clone)]
struct  Reminder {
    id: String,
    user_id: String,
    title: String,
    body: String,
    direction: Direction,
    start_date: Date<Utc>,
    stop_date: Date<Utc>,
    repetition: Repetition,
    day_schedule: DaySchedule,
}
impl Reminder {
    pub fn new(
        reminder_id: String,
        user_id: String,
        title: String,
        body: String,
        direction: Direction,
        start_date: Date<Utc>,
        stop_date: Date<Utc>,
        repetition: Repetition,
        day_schedule: DaySchedule,
    ) -> Self {
        Reminder {
            id: reminder_id,
            user_id: user_id,
            title: title,
            body: body,
            direction: direction,
            start_date: start_date,
            stop_date: stop_date,
            repetition: repetition,
            day_schedule: day_schedule,
        }
    }
    pub fn is_active(&self, now: &DateTime<Utc>) -> bool
    {
        ((now.date() > self.start_date) && (now.date() < self.stop_date)) &&
        self.day_schedule.is_active(now.hour())
    }
}

#[derive(Clone)]
enum Direction {
    Coming,
    Going,
    Both,
}

// #[derive(Clone, PartialEq, Eq, PartialOrd, Ord)]
// struct Date {
//     year: u16,
//     month: u8,
//     day: u8,
// }
// impl Date {
//     pub fn new(year: u16, month: u8, day: u8) -> Self {
//         Date{
//             year: year,
//             month: month,
//             day: day,
//         }
//     }
// }

#[derive(Clone)]
enum Repetition {
    OnceDaily,
    Daily,
    Weekly(Weekday),
    Monthly(u8),
    Yearly(u16),
}

#[derive(Clone)]
struct DaySchedule {
    start_hour: u8,
    stop_hour: u8,
}
impl DaySchedule {
    pub fn new(start_hour: Hour, stop_hour: Hour) -> Self {
        DaySchedule {
            start_hour: 0,
            stop_hour: 0,
        }
    }
    pub fn is_active(self, curr_hour: u8) -> bool {
        self.start_hour < curr_hour && self.stop_hour > curr_hour
    }
}

#[derive(Clone)]
struct Weekday {
    _internal: u8,
}
impl Weekday {
    pub fn new(
        sun: bool,
        mon: bool,
        tue: bool,
        wed: bool,
        thu: bool,
        fri: bool,
        sat: bool,
    ) -> Self {
        let mut retVal = Weekday { _internal: 0 };
        retVal.set_sun(sun);
        retVal.set_mon(mon);
        retVal.set_tue(tue);
        retVal.set_wed(wed);
        retVal.set_thu(thu);
        retVal.set_fri(fri);
        retVal.set_sat(sat);
        retVal
    }
    fn set_n(&mut self, en: bool, n: u8) {
        if en {
            self._internal |= 1 << n;
        } else {
            self._internal &= !(1 << n);
        }
    }
    pub fn set_sun(&mut self, en: bool) {
        self.set_n(en, 7);
    }
    pub fn set_mon(&mut self, en: bool) {
        self.set_n(en, 6);
    }
    pub fn set_tue(&mut self, en: bool) {
        self.set_n(en, 5);
    }
    pub fn set_wed(&mut self, en: bool) {
        self.set_n(en, 4);
    }
    pub fn set_thu(&mut self, en: bool) {
        self.set_n(en, 3);
    }
    pub fn set_fri(&mut self, en: bool) {
        self.set_n(en, 2);
    }
    pub fn set_sat(&mut self, en: bool) {
        self.set_n(en, 1);
    }
}
