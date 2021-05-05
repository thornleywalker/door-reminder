///////////////////////////////////////
// All references are to si115x-datasheet.pdf
///////////////////////////////////////

#ifndef SI115X_H
#define SI115X_H

// I2C Registers
enum I2C_REG {
  PART_ID = 0x0, // Returns DEVID (0x51 for the Si1151, 0x52 for the Si1152, 0x53 for the Si1153)
  REV_ID = 0x01, // Hardware Rev (0x[MAJOR][MINOR])
  MFR_ID = 0x02,
  INFO0 = 0x03,
  INFO1 = 0x04,
  HOSTIN3 = 0x07,
  HOSTIN2 = 0x08,
  HOSTIN1 = 0x09,
  HOSTIN0 = 0x0A,    // Data for parameter table on PARAM_SET write to COMMAND register
  COMMAND = 0x0B,    // Initiated action in Sensor when specific codes written here
  IRQ_ENABLE = 0x0F, // The six least significant bits enable Interrupt Operation
  RESPONSE1 = 0x10,  // Contains the readback value from a param query or a param set command.
  RESPONSE0 = 0x11,  // The 5th MSB of the counter is an error indicator, with the 4 LSBits
                     // indicating the error code when the MSB is set
  IRQ_STATUS = 0x12, // The six least significant bits show the interrupt status
  // Captured Sensor Data
  HOSTOUT0 = 0x13,
  HOSTOUT1 = 0x14,
  HOSTOUT2 = 0x15,
  HOSTOUT3 = 0x16,
  HOSTOUT4 = 0x17,
  HOSTOUT5 = 0x18,
  HOSTOUT6 = 0x19,
  HOSTOUT7 = 0x1A,
  HOSTOUT8 = 0x1B,
  HOSTOUT9 = 0x1C,
  HOSTOUT10 = 0x1D,
  HOSTOUT11 = 0x1E,
  HOSTOUT12 = 0x1F,
  HOSTOUT13 = 0x20,
  HOSTOUT14 = 0x21,
  HOSTOUT15 = 0x22,
  HOSTOUT16 = 0x23,
  HOSTOUT17 = 0x24,
  HOSTOUT18 = 0x25,
  HOSTOUT19 = 0x26,
  HOSTOUT20 = 0x27,
  HOSTOUT21 = 0x28,
  HOSTOUT22 = 0x29,
  HOSTOUT23 = 0x2A,
  HOSTOUT24 = 0x2B,
  HOSTOUT25 = 0x2C
};

// Command Register Commands
#define RESET_CMD_CTR 0x00 // Resets RESPONSE0 CMMND_CTR field to 0
#define RESET_SW 0x01      // Forces a Reset, Resets RESPONSE0 CMMND_CTR field to 0xXXX01111
#define FORCE                                                                                      \
  0x11 // Initiates a set of measurements specified in CHAN_LIST parameter. A FORCE command will
       // only execute the measurements which do not have a meas counter index configured in
       // MEASCONFIGx
#define PAUSE 0x12 // Pauses autonomous measurements specified in CHAN_LIST
#define START                                                                                      \
  0x13 // Starts autonomous measurements specified in CHAN_LIST.A START autonomous command will only
       // start the measurements which has a counter index selected in MEASCONFIGx
#define PARAM_QUERY(param)                                                                         \
  (0b01000000 | param) // Reads Parameter xxxxxx and store results in RESPONSE1.xxxxxx is a 6 bit
                       // Address Field (64 bytes).
#define PARAM_SET(param)                                                                           \
  (0b10000000 |                                                                                    \
   param) // Writes INPUT0 to the Parameter xxxxxx.xxxxxx is a 6 bit Address Field(64 bytes).

// Parameter table
typedef enum {
  // I2C Address (Temp)
  I2C_ADDR = 0x00,
  // Channel List, see 6.1
  CHAN_LIST = 0x01,
  // Channel 0 Setup
  ADCCONFIG0 = 0x02,
  ADCSENS0 = 0x03,
  ADCPOST0 = 0x04,
  MEASCONFIG0 = 0x05,
  // Channel 1 Setup
  ADCCONFIG1 = 0x06,
  ADCSENS1 = 0x07,
  ADCPOST1 = 0x08,
  MEASCONFIG1 = 0x09,
  // Channel 2 Setup
  ADCCONFIG2 = 0x0A,
  ADCSENS2 = 0x0B,
  ADCPOST2 = 0x0C,
  MEASCONFIG2 = 0x0D,
  // Channel 3 Setup
  ADCCONFIG3 = 0x0E,
  ADCSENS3 = 0x0F,
  ADCPOST3 = 0x10,
  MEASCONFIG3 = 0x11,
  // Channel 4 Setup
  ADCCONFIG4 = 0x12,
  ADCSENS4 = 0x13,
  ADCPOST4 = 0x14,
  MEASCONFIG4 = 0x15,
  // Channel 5 Setup
  ADCCONFIG5 = 0x16,
  ADCSENS5 = 0x17,
  ADCPOST5 = 0x18,
  MEASCONFIG5 = 0x19,
  // Time between measurements of a given channel is  800 us * MEASRATE * MEASCOUNT
  // MEASURE RATE
  MEASRATE_H = 0x1A, // Governs time between measurement periods
  MEASRATE_L = 0x1B,
  // MEASCOUNT
  MEASCOUNT1 = 0x1C, // Controls the number of periods between measurements
  MEASCOUNT2 = 0x1D,
  MEASCOUNT3 = 0x1E,
  // LED Setup
  LED1_A = 0x1F,
  LED1_B = 0x20,
  LED3_A = 0x21,
  LED3_B = 0x22,
  LED2_A = 0x23,
  LED2_B = 0x24,
  // THRESHOLD SETUP
  THRESHOLD0_H = 0x25,
  THRESHOLD0_L = 0x26,
  THRESHOLD1_H = 0x27,
  THRESHOLD1_L = 0x28,
  // THRESHOLD WINDOW SETUP
  UPPER_THRESHOLD_H = 0x29,
  UPPER_THRESHOLD_L = 0x2A,
  LOWER_THRESHOLD_H = 0x2C,
  LOWER_THRESHOLD_L = 0x2D,
  // BURST
  BURST = 0x2B,
} parameter_t;

// ADCCONFIG
enum ADCCONFIG_DECIM_RATE { DECIM_0, DECIM_1, DECIM_2, DECIM_3 };
enum ADCCONFIG_ADCMUX {
  SMALL_IR = 0b00000,
  MED_IR = 0b00001,
  LRG_IR = 0b00010,
  WHITE = 0b01011,
  LRG_WHITE = 0b01101
};
#define _DECIM_RATE(n) (n << 4)
#define ADCCONFIG(decim_rate, adcmux) (_DECIM_RATE(decim_rate) | adcmux)

// ADCSENS
enum ADCSENS_HSIG { NORMAL_GAIN = 0b0, HIGH_GAIN = 0b1 };
#define ADCSENS(hsig, sw_gain, hw_gain) (hsig << 7 | sw_gain << 4 | hw_gain)

// ADCPOST
///// DO NOT SET 24-BIT MODE WHTN USING THE THRESHOLD /////
enum ADCPOST_24BIT_OUT { U_16 = 0, S_24 = 1 };
enum ADCPOST_THRESH_POL {
  GREATER_THAN = 0, // used with THRESH_0 and THRESH_1 below
  LESS_THAN = 1,    // used with THRESH_0 and THRESH_1 below
  EXIT_WINDOW = 0,  // used with THRESH_WINDOW below
  ENTER_WINDOW = 1  // used with THRESH_WINDOW below
};
enum ADCPOST_THRESH_EN { // see 6.4
  NO_THRESH = 0,         // interrupts on every sample
  THRESH_0 = 1,          // channel will use THRESHOLD0
  THRESH_1 = 2,          // channel will use THRESHOLD1
  THRESH_WINDOW = 3 // channel will use windows determined by UPPER_THRESHOLD and LOWER_THRESHOLD
};
#define ADCPOST(bit_out_24, post_shift, thresh_pol, thresh_sel)                                    \
  (bit_out_24 << 6 | post_shift << 3 | thresh_pol << 2 | thresh_sel)

// MEASCONFIG
enum MEASCONFIG_COUNTER_INDEX { BURST_FORCED = 0, MC_0 = 1, MC_1 = 2, MC_2 = 3 };
enum MEASCONFIG_LED_TRIM { NOMINAL = 0, INC_9 = 2, DEC_10 = 3 };
enum MEASCONFIG_BANK_SEL { BANK_A = 0, BANK_B = 1 };
enum MEASCONFIG_LEDS { LED_1 = 1, LED_2 = 2, LED_ = 4 };
#define MEASCONFIG(counter_index, led_trim, bank_sel, leds)                                        \
  (counter_index << 6 | led_trim << 4 | bank_sel << 3 | leds)

// Configures the given channel and enables it
// args:
//   channel     - the channel to apply the configuration to
//   adc_config  - the adc configuration to use (use ADCCONFIG macro) - see 7.2.1
//   adc_sens    - the adc sensor gains to use (use ADCSENS macro) - see 7.2.2
//   adc_post    - the adc threshold configuration to use (use ADCPOST macro) - see 7.2.3
//   meas_config - the measurement/LED configuration to use (use MEASCONFIG macro) - see 7.2.4
// example usage:
//     si115x_channel_config(
//         2,
//         ADCCONFIG(DECIM_0, LRG_IR),
//         ADCSENS(NORMAL_GAIN, 3, 7),
//         ADCPOST(U_16, 3, ENTER_WINDOW, THRESH_WINDOW),
//         MEASCONFIG(MC_0, NOMINAL, BANK_B, LED_1 | LED_2)
//         );
void si115x_channel_config(int i2c_address, int channel, char adc_config, char adc_sens,
                           char adc_post, char meas_config);

// Issues given command, see 5.1
// For parameter table queries and sets, use read_param/write_param
// args:
//  i2c_address - i2c address of the device
//  command - the command to give
// return: 0 if successful, see 5.4.10 for error codes
int si115x_issue_command(int i2c_address, char command);

// Reads the desired parameter from the parameter table, see 5.3.1
// args:
//  i2c_address - i2c address of the device
//  param - parameter to read, see parameter_t
//  buf - buffer to store value in
// return: 0 if successful, see 5.4.10 for error codes
int si115x_read_param(int i2c_address, parameter_t param, char *buf);

// Writes the given value to the parameter in the parameter table, see 5.3.1
// args:
//  i2c_address - i2c address of the device
//  param - the parameter to write, see parameter_t
//  value - the value of the parameter to write
// return: 0 if successful, see 5.4.10 for error codes
int si115x_write_param(int i2c_address, parameter_t param, char value);

// Reads the current register value
// args:
//  i2c_address - i2c address of the device
//  reg_address - address of the register to read, see I2C_REG enum
// return: value of the register
void si115x_read_reg(int i2c_address, char reg_address, char *buf);

// Writes the value to the given register
// args:
//  i2c_address - i2c address of the device
//  reg_address - address of the register to read, see I2C_REG enum
//  value - the value to be written
void si115x_write_reg(int i2c_address, char reg_address, char value);

#endif