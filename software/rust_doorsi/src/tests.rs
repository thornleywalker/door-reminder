use crate::*;
#[test]
fn basic_test() {
    let mut doorsi: Doorsi = Doorsi::new();
    let test_user = User::new("".to_string());
    test_user.reminders.push(Reminder::new(
        "".to_string(),
        "".to_string(),
        "".to_string(),
        "".to_string(),
        Direction::Going,
        Date::new(0, 0, 0),
        Date::new(0, 0, 0),
        Repetition::new(),
        DaySchedule::new(),
    ));

    doorsi.add_user(test_user);

}