use std::sync::Arc;

use esp_idf_sys as _; // If using the `binstart` feature of `esp-idf-sys`, always keep this module imported
use esp_idf_sys::EspError;

use esp_idf_svc::{netif::*, nvs::EspDefaultNvs, sysloop::*, wifi::*};

use embedded_svc::wifi::*;

use log::*;

const SSID: &str = "WeeFee";
const PASS: &str = "P@ssw0rd";

pub struct DoorsiWifi {
    _internal: Box<EspWifi>,
}
impl DoorsiWifi {
    pub fn new(
        netif_stack: Arc<EspNetifStack>,
        sys_loop_stack: Arc<EspSysLoopStack>,
        default_nvs: Arc<EspDefaultNvs>,
    ) -> Result<Self, EspError> {
        let mut wifi = Box::new(EspWifi::new(netif_stack, sys_loop_stack, default_nvs)?);
        info!("Wifi created, about to scan");
        let mut ret_val = Self { _internal: wifi };

        // ret_val.connect_to(SSID.to_string(), PASS.to_string())?;

        Ok(ret_val)
    }
    pub fn connect_to(&mut self, ssid: String, pass: String) -> Result<(), EspError> {
        let mut wifi = &mut self._internal;
        let ap_infos = wifi.scan()?;
        let ours = ap_infos.into_iter().find(|a| a.ssid == ssid);
        let channel = if let Some(ours) = ours {
            info!(
                "Found configured access point {} on channel {}",
                ssid, ours.channel
            );
            Some(ours.channel)
        } else {
            info!(
                "Configured access point {} not found during scanning, will go with unknown channel",
                ssid
            );
            None
        };
        wifi.set_configuration(&Configuration::Mixed(
            ClientConfiguration {
                ssid: ssid.into(),
                password: pass.into(),
                channel,
                ..Default::default()
            },
            AccessPointConfiguration {
                ssid: "aptest".into(),
                channel: channel.unwrap_or(1),
                ..Default::default()
            },
        ))?;
        info!("Wifi configuration set, about to get status");
        let status = wifi.get_status();
        if let Status(
            ClientStatus::Started(ClientConnectionStatus::Connected(ClientIpStatus::Done(
                ip_settings,
            ))),
            ApStatus::Started(ApIpStatus::Done),
        ) = status
        {
            info!("Wifi connected");
            //ping(&ip_settings)?;
        } else {
            //bail!("Unexpected Wifi status: {:?}", status);
        }
        Ok(())
    }
}
