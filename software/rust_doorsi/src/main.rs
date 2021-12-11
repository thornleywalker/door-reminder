use esp_idf_sys as _; // If using the `binstart` feature of `esp-idf-sys`, always keep this module imported

mod doorsi;
use doorsi::Doorsi;

fn main() -> ! {
    // Temporary. Will disappear once ESP-IDF 4.4 is released, but for now it is necessary to call this function once,
    // or else some patches to the runtime implemented by esp-idf-sys might not link properly.
    esp_idf_sys::link_patches();

    // Bind the log crate to the ESP Logging facilities
    esp_idf_svc::log::EspLogger::initialize_default();

    // setup wifi
    // set time
    // setup bluetooth
    // setup updater
    // setup Doorsi
    let mut doorsi = Doorsi::new();
    doorsi.run()
}
