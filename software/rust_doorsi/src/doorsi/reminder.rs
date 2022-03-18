use chrono::{DateTime, NaiveDate, NaiveDateTime, NaiveTime};

#[derive(Clone)]
pub struct Reminder {
    id: String,
    pub user_id: String,
    title: String,
    body: String,
    direction: Direction,
    schedule: Schedule,
}
impl Reminder {
    pub fn new(
        reminder_id: String,
        user_id: String,
        title: String,
        body: String,
        direction: Direction,
        schedule: Schedule,
    ) -> Self {
        Reminder {
            id: reminder_id,
            user_id: user_id,
            title: title,
            body: body,
            direction: direction,
            schedule: schedule,
        }
    }
    pub fn trigger(&mut self) {
        if self.is_active() {}
    }
    fn is_active(&self) -> bool {
        false
    }
}

#[derive(Clone)]
pub enum Direction {
    Coming,
    Going,
    Both,
}

pub struct Schedule {
    start_date: NaiveDate,
    stop_date: NaiveDate,
    day_schedule: DaySchedule,
    repetition: Repetition,
}

#[derive(Clone)]
pub enum Repetition {
    OnceDaily,
    Daily,
    Weekly(Weekdays),
    Monthly(u8), // day of the month
    Yearly(u16), // day of the year
}

type Hour = u8;

#[derive(Clone)]
pub struct DaySchedule {
    start_hour: Hour,
    stop_hour: Hour,
}
impl DaySchedule {
    pub fn new(start_hour: Hour, stop_hour: Hour) -> Self {
        DaySchedule {
            start_hour: start_hour,
            stop_hour: stop_hour,
        }
    }
    pub fn is_active(&self, curr_hour: Hour) -> bool {
        self.start_hour <= curr_hour && curr_hour < self.stop_hour
    }
}

#[derive(Clone)]
pub struct Weekdays {
    _internal: u8,
}
impl Weekdays {
    pub fn new(
        sun: bool,
        mon: bool,
        tue: bool,
        wed: bool,
        thu: bool,
        fri: bool,
        sat: bool,
    ) -> Self {
        let mut ret_val = Weekdays { _internal: 0 };
        ret_val.set_sun(sun);
        ret_val.set_mon(mon);
        ret_val.set_tue(tue);
        ret_val.set_wed(wed);
        ret_val.set_thu(thu);
        ret_val.set_fri(fri);
        ret_val.set_sat(sat);
        ret_val
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
