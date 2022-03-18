use std::{
    sync::Arc,
    thread,
    time::{Duration, SystemTime},
};

use esp_idf_sys as _; // If using the `binstart` feature of `esp-idf-sys`, always keep this module imported
use esp_idf_sys::EspError;

use esp_idf_svc::{netif::*, nvs::EspDefaultNvs, sntp::*, sysloop::*};

use log::*;

mod doorsi;
mod wifi;

use doorsi::Doorsi;
use wifi::DoorsiWifi;

fn main() -> Result<(), EspError> {
    // Temporary. Will disappear once ESP-IDF 4.4 is released, but for now it is necessary to call this function once,
    // or else some patches to the runtime implemented by esp-idf-sys might not link properly.
    esp_idf_sys::link_patches();

    // Bind the log crate to the ESP Logging facilities
    esp_idf_svc::log::EspLogger::initialize_default();

    println!(
        "Now: {}",
        SystemTime::now()
            .duration_since(SystemTime::UNIX_EPOCH)
            .unwrap()
            .as_secs()
    );

    // setup nvs
    let default_nvs = Arc::new(EspDefaultNvs::new()?);

    // setup bluetooth, first line of communication with phone for setup

    // setup wifi
    let netif_stack = Arc::new(EspNetifStack::new()?);
    let sys_loop_stack = Arc::new(EspSysLoopStack::new()?);

    let mut wifi = DoorsiWifi::new(
        netif_stack.clone(),
        sys_loop_stack.clone(),
        default_nvs.clone(),
    )?;

    wifi.connect_to("WeeFee".to_string(), "P@ssw0rd".to_string());

    // set time
    let sntp = EspSntp::new_default()?;

    // setup Doorsi
    let mut doorsi = Doorsi::new()?;
    // let mut doorsi = match Doorsi::new() {
    //     Ok(val) => val,
    //     Err(e) => {
    //         info!("error: {:?}", e);
    //         panic!();
    //     }
    // };
    doorsi.run();

    loop {
        println!("Sync Status: {}", sntp.get_sync_status() as u8);
        println!(
            "Now: {}",
            SystemTime::now()
                .duration_since(SystemTime::UNIX_EPOCH)
                .unwrap()
                .as_secs()
        );
        thread::sleep(Duration::from_secs(5));
    }
    // setup updater

    // for i in 0..1000 {
    //     info!("round: {}", i);
    // }
    // loop {
    // }
    Ok(())
}

// fn setup_wifi(
//     netif_stack: Arc<EspNetifStack>,
//     sys_loop_stack: Arc<EspSysLoopStack>,
//     default_nvs: Arc<EspDefaultNvs>,
// ) -> Result<Box<EspWifi>, EspError> {
//     let mut wifi = Box::new(EspWifi::new(netif_stack, sys_loop_stack, default_nvs)?);

//     info!("Wifi created, about to scan");

//     let ap_infos = wifi.scan()?;

//     let ours = ap_infos.into_iter().find(|a| a.ssid == SSID);

//     let channel = if let Some(ours) = ours {
//         info!(
//             "Found configured access point {} on channel {}",
//             SSID, ours.channel
//         );
//         Some(ours.channel)
//     } else {
//         info!(
//             "Configured access point {} not found during scanning, will go with unknown channel",
//             SSID
//         );
//         None
//     };

//     wifi.set_configuration(&Configuration::Mixed(
//         ClientConfiguration {
//             ssid: SSID.into(),
//             password: PASS.into(),
//             channel,
//             ..Default::default()
//         },
//         AccessPointConfiguration {
//             ssid: "aptest".into(),
//             channel: channel.unwrap_or(1),
//             ..Default::default()
//         },
//     ))?;

//     info!("Wifi configuration set, about to get status");

//     let status = wifi.get_status();

//     if let Status(
//         ClientStatus::Started(ClientConnectionStatus::Connected(ClientIpStatus::Done(ip_settings))),
//         ApStatus::Started(ApIpStatus::Done),
//     ) = status
//     {
//         info!("Wifi connected");

//         //ping(&ip_settings)?;
//     } else {
//         //bail!("Unexpected Wifi status: {:?}", status);
//     }

//     Ok(wifi)
// }
