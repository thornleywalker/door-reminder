use chrono::{DateTime, Date, Utc, Timelike};

#[derive(Clone)]
pub struct  Reminder {
    id: String,
    pub user_id: String,
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
        self.day_schedule.is_active(now.time().hour() as Hour)
    }
}

#[derive(Clone)]
pub enum Direction {
    Coming,
    Going,
    Both,
}

#[derive(Clone)]
pub enum Repetition {
    OnceDaily,
    Daily,
    Weekly(Weekday),
    Monthly(u8),
    Yearly(u16),
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
            start_hour: 0,
            stop_hour: 0,
        }
    }
    pub fn is_active(&self, curr_hour: Hour) -> bool {
        self.start_hour < curr_hour && self.stop_hour > curr_hour
    }
}

#[derive(Clone)]
pub struct Weekday {
    _internal: Hour,
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
        let mut ret_val = Weekday { _internal: 0 };
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