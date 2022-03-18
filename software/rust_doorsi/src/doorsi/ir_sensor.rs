use esp_idf_sys::*;
use esp_idf_hal::{i2c::{self, I2c, I2C0, Master, MasterPins}, gpio::*, prelude::*};
use log::*;
use si115x::*;

type SdaPin = Gpio4<Unknown>;
type SclPin = Gpio16<Unknown>;

pub struct IrSensor {
    address: u8,
}

impl IrSensor {
    pub fn new<I: I2c, A: OutputPin + InputPin, L: OutputPin>(
        addr: u8,
        i2c: &mut Master<I, A, L>,
    ) -> Result<Self, EspError> {
        info!("Creating IR Sensor at {}", addr);
        let channel = Channel::new(
            AdcConfig::new(
                DecimRate::Clks1024,
                AdcMux::LargeIr,
            ),
            AdcSens::new(
                Hsig::NormalGain,
                SwGain::MeasCount16,
                HwGain::Ms1p5,
            ),
            AdcPost::new(
                BitsOut::Bits24,
                0,
                ThreshCfg::EntersWindow
            ),
            MeasConfig::new(
                CounterIndex::MeasCount0,
                LedTrim::Nominal,
                BankSel::A,
                false,
                false,
                true,
            ),
        );
        
        let ret_val = Self {
            address: addr,
        };

        info!("command vec defined");
        let command_vec = [
            (Command::ParamSet(Parameter::ADCConfig0), Some(channel.adc_config.into())),
            (Command::ParamSet(Parameter::ADCSens0), Some(channel.adc_sens.into())),
            (Command::ParamSet(Parameter::ADCPost0), Some(channel.adc_post.into())),
            (Command::ParamSet(Parameter::MeasConfig0), Some(channel.meas_config.into()))
            ];
        info!("command vec defined");

        info!("Executing commands");
        for (command, val) in command_vec {
            let resp = ret_val.do_command(i2c, command, val)?;
            match resp.status.cmd_ctr {
                Err(e) => error!("i2c command didn't work :/"),
                _ => {}
            }
        }

        info!("IR Sensor created");
        Ok(ret_val)
    }
    fn do_command<I: I2c, A: OutputPin + InputPin, L: OutputPin>(
        &self,
        i2c: &mut Master<I, A, L>,
        command: Command,
        value: Option<u8>
    ) -> Result<Response, EspError> {
        info!("Beginning command");
        // Read Resonse0 reg, store command counter
        let bytes = [Register::Response0.into()];
        info!("Got here0.1");
        let mut buffer = [0; 1];
        info!("Got here0.2");
        i2c.write_read(self.address, &bytes, &mut buffer)?;

        info!("Got here1");
        let pre_response0 = Response0::from(buffer[0]);
        let mut pre_count = 0;
        match pre_response0.cmd_ctr {
            Ok(count) => pre_count = count,
            Err(e) => return Ok(Response { value: 0, status: pre_response0 }),
        }
        info!("Got here2");

        // write value to Hostin reg (option)
        // write command to Command reg
        match value {
            Some(val) => {
                let bytes = [Register::HostIn0.into(), val, command.into()];
                i2c.write(self.address, &bytes)?;
            },
            None => {
                let bytes = [Register::Command.into(), command.into()];
                i2c.write(self.address, &bytes)?;
            },
        };
        i2c.write(self.address, &bytes)?;
        info!("Got here3");

        // Read Response0 reg, check command counter incremented
        let bytes = [Register::Response1.into()];
        let mut buffer = [0; 2];

        loop {
            i2c.write_read(self.address, &bytes, &mut buffer).unwrap();
            let mut post_response = Response0::from(buffer[1]);
            match post_response.cmd_ctr {
                Ok(count) => if count > pre_count {
                    break;
                },
                Err(e) => break,
            }
        }
        info!("Got here4");

        info!("Command completed");
        // Repeat until counter increments, or error
        Ok(Response {
            value: buffer[0],
            status: buffer[1].into(),
        })
    }
}

pub struct IrManager {
    i2c_master: Master<I2C0, SdaPin, SclPin>,
    left_ir: IrSensor,
    right_ir: IrSensor,
}
impl IrManager {
    // creates and initializes the ir sensors
    pub fn new(
        i2c: I2C0,
        sda: SdaPin,
        scl: SclPin,
    ) -> Result<Self, EspError> {
        info!("Creating IR Manager");
        let config = <i2c::config::MasterConfig as Default>::default().baudrate(400.kHz().into());
        let mut i2c_master = Master::<I2C0, _, _>::new(
            i2c,
            MasterPins { sda, scl },
            config,
        )?;
        
        // do initialization of sensors
        let left_ir = IrSensor::new(52, &mut i2c_master)?;
        let right_ir = IrSensor::new(51, &mut i2c_master)?;

        Ok(Self {
            i2c_master: i2c_master,
            left_ir: left_ir,
            right_ir: right_ir,
        })
    }
}

mod si115x {
    #[derive(Copy, Clone)]
    pub enum CmdCtrCode {
        InvalidCommand = 0x10,
        InvalidLocation = 0x11,
        AdcSaturation = 0x12,
        OutputOverflow = 0x13,
    }

    #[derive(Copy, Clone)]
    pub struct Response {
        pub value: u8,
        pub status: Response0,
    }

    #[derive(Copy, Clone)]
    pub struct Response0 {
        pub running: bool,
        pub suspend: bool,
        pub sleep: bool,
        pub cmd_ctr: Result<u8, CmdCtrCode>,
    }
    impl From<u8> for Response0 {
        fn from(byte: u8) -> Self {
            Self {
                running: (byte & 0x80) == 0x80,
                suspend: (byte & 0x40) == 0x40,
                sleep: (byte & 0x20) == 0x20,
                cmd_ctr: match (byte & 0x10) {
                    0x10 => Err(
                        match (byte & 0x0F) {
                            0x10 => CmdCtrCode::InvalidCommand,
                            0x11 => CmdCtrCode::InvalidLocation,
                            0x12 => CmdCtrCode::AdcSaturation,
                            0x13 => CmdCtrCode::OutputOverflow,
                            _ => panic!("Unexpected command response."),
                        }
                    ),
                    0x00 => Ok(byte & 0x0F),
                    _ => panic!("unexpected command response."),
                }
            }
        }
    }

    #[derive(Copy, Clone)]
    pub enum Register {
        PartId = 0x0, // IN
        RevId = 0x1, // IN
        MfrId = 0x2, // IN
        Info0 = 0x3, // IN
        Info1 = 0x4, // IN
        HostIn3 = 0x7, // IN/OUT
        HostIn2 = 0x8, // IN/OUT
        HostIn1 = 0x9, // IN/OUT
        HostIn0 = 0x0A, // IN/OUT
        Command = 0x0B, // IN/OUT
        IrqEnable = 0x0F, // IN/OUT
        Response1 = 0x10, // IN
        Response0 = 0x11, // IN
        IrqStatus = 0x12, // IN
        HostOut0 = 0x13, // IN
        HostOut1 = 0x14, // IN
        HostOut2 = 0x15, // IN
        HostOut3 = 0x16, // IN
        HostOut4 = 0x17, // IN
        HostOut5 = 0x18, // IN
        HostOut6 = 0x19, // IN
        HostOut7 = 0x1A, // IN
        HostOut8 = 0x1B, // IN
        HostOut9 = 0x1C, // IN
        HostOut10 = 0x1D, // IN
        HostOut11 = 0x1E, // IN
        HostOut12 = 0x1F, // IN
        HostOut13 = 0x20, // IN
        HostOut14 = 0x21, // IN
        HostOut15 = 0x22, // IN
        HostOut16 = 0x23, // IN
        HostOut17 = 0x24, // IN
        HostOut18 = 0x25, // IN
        HostOut19 = 0x26, // IN
        HostOut20 = 0x27, // IN
        HostOut21 = 0x28, // IN
        HostOut22 = 0x29, // IN
        HostOut23 = 0x2A, // IN
        HostOut24 = 0x2B, // IN
        HostOut25 = 0x2C, // IN
    }
    impl From<Register> for u8 {
        fn from(r: Register) -> Self {
            r as u8
        }
    }

    #[derive(Copy, Clone)]
    pub enum Parameter {
        I2CAddr = 0x00,
        ChanList = 0x01,
        ADCConfig0 = 0x02,
        ADCSens0 = 0x03,
        ADCPost0 = 0x04,
        MeasConfig0 = 0x05,
        ADCConfig1 = 0x06,
        ADCSens1 = 0x07,
        ADCPost1 = 0x08,
        MeasConfig1 = 0x09,
        ADCConfig2 = 0x0A,
        ADCSens2 = 0x0B,
        ADCPost2 = 0x0C,
        MeasConfig2 = 0x0D,
        ADCConfig3 = 0x0E,
        ADCSens3 = 0x0F,
        ADCPost3 = 0x10,
        MeasConfig3 = 0x11,
        ADCConfig4 = 0x12,
        ADCSens4 = 0x13,
        ADCPost4 = 0x14,
        MeasConfig4 = 0x15,
        ADCConfig5 = 0x16,
        ADCSens5 = 0x17,
        ADCPost5 = 0x18,
        MeasConfig5 = 0x19,
        MeasRateH = 0x1A,
        MeasRateL = 0x1B,
        MeasCount0 = 0x1C,
        MeasCount1 = 0x1D,
        MeasCount2 = 0x1E,
        Led1A = 0x1F,
        Led1B = 0x20,
        Led3A = 0x21,
        Led3B = 0x22,
        Led2A = 0x23,
        Led2B = 0x24,
        Threshold0H = 0x25,
        Threshold0L = 0x26,
        Threshold1H = 0x27,
        Threshold1L = 0x28,
        UpperTresholdH = 0x29,
        UpperTresholdL = 0x2A,
        Burst = 0x2B,
        LowerThresholdH = 0x2C,
        LowerThresholdL = 0x2D,
    }

    #[derive(Copy, Clone)]
    pub enum Command {
        ResetCmdCtr,
        ResetSw,
        Force,
        Pause,
        Start,
        ParamQuery(Parameter),
        ParamSet(Parameter),
    }

    impl From<Command> for u8 {
        fn from(command: Command) -> Self {
            match command {
                Command::ParamQuery(param) => 0x40 | (param as u8),
                Command::ParamSet(param) => 0x80 | (param as u8),
                Command::ResetCmdCtr => 0x00,
                Command::ResetSw => 0x01,
                Command::Force => 0x11,
                Command::Pause => 0x12,
                Command::Start => 0x13,
            }
        }
    }

    /* Selects Decimations rate of A/Ds. This setting affects the number of clocks used
    per measurements. Decimation rate is an A/D optimization parameter. The most common
    decimation value is 0 for a 1024 clocks and 48.8 μs min measurement time. Consult the
    related application notes for more details. Increasing the reading time by using more
    clocks does not cause the ADC count to be larger.
    */
    #[derive(Copy, Clone)]
    pub enum DecimRate {
        Clks1024 = 0, // 1024 * 21MHz clocks, normal
        Clks2048 = 1, // 2048 * 21MHz clocks
        Clks4096 = 2, // 4096 * 21MHz clocks
        Clks512 = 3, // 512 * 21MHz clocks
    }

    /*The ADC Mux selects which photodiode(s) are connected to the ADCs for measurement.
    See Photodiode Section for more information regarding the location of the photodiodes.
    */
    #[derive(Copy, Clone)]
    pub enum AdcMux {
        SmallIr = 0,
        MediumIr = 1,
        LargeIr = 2,
        White = 11,
        LargeWhite = 13,
    }
    #[derive(Copy, Clone)]
    pub struct AdcConfig {
        decim_rate: DecimRate,
        adc_mux: AdcMux,
    }
    impl AdcConfig {
        pub fn new(decim_rate: DecimRate, adc_mux: AdcMux) -> Self {
            Self {
                decim_rate: decim_rate,
                adc_mux: adc_mux,
            }
        }
        pub fn to_byte(&self) -> u8 {
            (self.decim_rate as u8) << 5 |
            (self.adc_mux as u8)
        }
    }
    impl From<AdcConfig> for u8 {
        fn from(val: AdcConfig) -> Self {
            val.to_byte()
        }
    }

    /* This is the Ranging bit for the A/D. Normal gain at 0 and High range (sensitivity is
    divided by 14.5) when set to 1.
    */
    #[derive(Copy, Clone)]
    pub enum Hsig {
        NormalGain = 0,
        HighRange = 1,
    }
    
    /* Causes an internal accumulation of samples with no pause between readings when in
    FORCED Mode. In Autonomous mode the the accumulation happens at the measurement rate
    selected. The calculations are accumulated in 24 bits and an optional shift is applied
    later. See ADCPOSTx.ADC_MISC[1:0]
    */
    #[derive(Copy, Clone)]
    pub enum SwGain {
        MeasCount1 = 0,
        MeasCount2 = 1,
        MeasCount4 = 2,
        MeasCount8 = 3,
        MeasCount16 = 4,
        MeasCount32 = 5,
        MeasCount64 = 6,
        MeasCount128 = 7,
    }

    /* Nominal Measurement time for 512 clocks
    */
    #[derive(Copy, Clone)]
    pub enum HwGain {
        Us24p4 = 0,
        Us48p8 = 1,
        Us97p5 = 2,
        Us195 = 3,
        Us390 = 4,
        Us780 = 5,
        Ms1p5 = 6,
        Ms3 = 7,
        Ms6 = 8,
        Ms12 = 9,
        Ms25 = 10,
        Ms50 = 11,
    }

    #[derive(Copy, Clone)]
    pub struct AdcSens {
        hsig: Hsig,
        sw_gain: SwGain,
        hw_gain: HwGain,
    }
    impl AdcSens {
        pub fn new(
            hsig: Hsig,
            sw_gain: SwGain,
            hw_gain: HwGain,
        ) -> Self {
            Self {
                hsig: hsig,
                sw_gain: sw_gain,
                hw_gain: hw_gain,
            }
        }
        pub fn to_byte(&self) -> u8 {
            (self.hsig as u8) << 7 |
            (self.sw_gain as u8) << 4 |
            (self.hw_gain as u8)
        }
    }
    impl From<AdcSens> for u8 {
        fn from(val: AdcSens) -> Self {
            val.to_byte()
        }
    }

    #[derive(Copy, Clone)]
    pub enum BitsOut {
        Bits16 = 0,
        Bits24 = 1,
    }

    #[derive(Copy, Clone)]
    pub enum ThreshCfg {
        NoThresh = 0x0,
        LargerThanThresh0 = 0x1,
        SmallerThanThresh0 = 0x5,
        LargerThanThresh1 = 0x2,
        SmallerThanThresh1 = 0x6,
        EntersWindow = 0x7,
        ExitsWindow = 0x3,
    }

    #[derive(Copy, Clone)]
    pub struct AdcPost {
        bits_out: BitsOut,
        post_shift: u8,
        thresh_cfg: ThreshCfg,
    }
    impl AdcPost {
        pub fn new(
            bits_out: BitsOut,
            post_shift: u8,
            thresh_cfg: ThreshCfg,
        ) -> Self {
            Self {
                bits_out: bits_out,
                post_shift: post_shift,
                thresh_cfg: thresh_cfg,
            }
        }
        pub fn to_byte(&self) -> u8 {
            (self.bits_out as u8) << 6 |
            (self.post_shift as u8) << 3 |
            (self.thresh_cfg as u8)
        }
    }
    impl From<AdcPost> for u8 {
        fn from(val: AdcPost) -> Self {
            val.to_byte()
        }
    }

    #[derive(Copy, Clone)]
    pub enum CounterIndex {
        BurstForced = 0,
        MeasCount0 = 1,
        MeasCount1 = 2,
        MeasCount2 = 3,
    }

    #[derive(Copy, Clone)]
    pub enum LedTrim {
        Nominal = 0,
        Inc9 = 2,
        Dec10 = 3,
    }

    #[derive(Copy, Clone)]
    pub enum BankSel {
        A = 0,
        B = 1
    }

    #[derive(Copy, Clone)]
    pub struct MeasConfig {
        counter_index: CounterIndex,
        led_trim: LedTrim,
        bank_sel: BankSel,
        led2_en: bool,
        led3_en: bool,
        led1_en: bool,
    }
    impl MeasConfig {
        pub fn new(
            counter_index: CounterIndex,
            led_trim: LedTrim,
            bank_sel: BankSel,
            led2_en: bool,
            led3_en: bool,
            led1_en: bool,
        ) -> Self {
            Self {
                counter_index: counter_index,
                led_trim: led_trim,
                bank_sel: bank_sel,
                led2_en: led2_en,
                led3_en: led3_en,
                led1_en: led1_en,
            }
        }
        pub fn to_byte(&self) -> u8 {
            (self.counter_index as u8) << 6 |
            (self.led_trim as u8) << 4 |
            (self.bank_sel as u8) << 3 |
            (self.led2_en as u8) << 2 |
            (self.led3_en as u8) << 1 |
            (self.led1_en as u8)
        }
    }
    impl From<MeasConfig> for u8 {
        fn from(val: MeasConfig) -> Self {
            val.to_byte()
        }
    }

    #[derive(Copy, Clone)]
    pub struct Channel {
        pub adc_config: AdcConfig,
        pub adc_sens: AdcSens,
        pub adc_post: AdcPost,
        pub meas_config: MeasConfig,
    }
    impl Channel {
        pub fn new(
            adc_config: AdcConfig,
            adc_sens: AdcSens,
            adc_post: AdcPost,
            meas_config: MeasConfig
        ) -> Self {
            Self {
                adc_config: adc_config,
                adc_sens: adc_sens,
                adc_post: adc_post,
                meas_config: meas_config,
            }
        }
        fn to_bytes(&self) -> Vec<u8> {
            vec![
                self.adc_config.to_byte(),
                self.adc_sens.to_byte(),
                self.adc_post.to_byte(),
                self.meas_config.to_byte()
                ]
        }
    }
}
