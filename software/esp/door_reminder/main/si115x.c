#include "si115x.h"

////////////////////////////////////////////
///// Platform dependent section begin /////
////////////////////////////////////////////
#include "driver/i2c.h"

static int i2c_master_port = 0;

// Performs all necessary initializations
void si115x_init(int port_num, int sda_pin, int scl_pin, int freq) {
  // Initialize i2c
  i2c_master_port = port_num;
  i2c_config_t conf = {
      .mode = I2C_MODE_MASTER,
      .sda_io_num = sda_pin, // select GPIO specific to your project
      .sda_pullup_en = GPIO_PULLUP_ENABLE,
      .scl_io_num = scl_pin, // select GPIO specific to your project
      .scl_pullup_en = GPIO_PULLUP_ENABLE,
      .master.clk_speed = freq // select frequency specific to your project
  };
  i2c_param_config(i2c_master_port, conf);

  i2c_driver_install(i2c_master_port, I2C_MODE_MASTER, 0, 0, 0);
}

// Wrapper for i2c function
// Reads the current register value
// args:
//  i2c_address - i2c address of the device
//  reg_address - address of the register to read, see I2C_REG enum
// return: value of the register
inline void si115x_write_reg(char i2c_address, char reg_address, char value) {
  // Implement i2c write
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();

  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (i2c_address << 1) | 0x00, 1);
  i2c_master_write_byte(cmd, reg_address, 1);
  i2c_master_write_byte(cmd, value, 1);
  i2c_master_stop(cmd);

  i2c_master_cmd_begin(i2c_master_port, cmd, pdMS_TO_TICKS(50));
  i2c_cmd_link_delete(cmd);
}

// Wrapper for i2c function
// Writes the value to the given register
// args:
//  i2c_address - i2c address of the device
//  reg_address - address of the register to read, see I2C_REG enum
//  value - the value to be written
inline void si115x_read_reg(char i2c_address, char reg_address, char *buf) {
  // implement i2c read
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();

  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (i2c_address << 1) | 0x00, 1);
  i2c_master_write_byte(cmd, reg_address, 1);
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (i2c_address << 1) | 0x01, 1);
  i2c_master_read_byte(cmd, buf, 1);
  i2c_master_stop(cmd);

  i2c_master_cmd_begin(i2c_master_port, cmd, pdMS_TO_TICKS(50));
  i2c_cmd_link_delete(cmd);
}
//////////////////////////////////////////
///// Platform dependent section end /////
//////////////////////////////////////////

// Configures the given channel and enables it
// args:
//   channel     - the channel to apply the configuration to
//   adc_config  - the adc configuration to use (use ADCCONFIG macro) - see 7.2.1
//   adc_sens    - the adc sensor gains to use (use ADCSENS macro) - see 7.2.2
//   adc_post    - the adc threshold configuration to use (use ADCPOST macro) - see 7.2.3
//   meas_config - the measurement/LED configuration to use (use MEASCONFIG macro) - see 7.2.4
// example usage:
//     si115x_channel_config(
//         0x52,
//         2,
//         ADCCONFIG(DECIM_0, LRG_IR),
//         ADCSENS(NORMAL_GAIN, 3, 7),
//         ADCPOST(U_16, 3, ENTER_WINDOW, THRESH_WINDOW),
//         MEASCONFIG(MC_0, NOMINAL, BANK_B, LED_1 | LED_2)
//         );
void si115x_channel_config(int i2c_address, int channel, char adc_config, char adc_sens,
                           char adc_post, char meas_config) {
  // Check for valid channel
  if (channel >= 0 && channel <= 5) {
    int channel_offset = 0x02 + (channel * 0x04);
    si115x_write_param(i2c_address, channel_offset++, adc_config);
    si115x_write_param(i2c_address, channel_offset++, adc_sens);
    si115x_write_param(i2c_address, channel_offset++, adc_post);
    si115x_write_param(i2c_address, channel_offset, meas_config);
  }
}

// Issues given command, see 5.3
// Blocks until command is completed, or an error is given
// For parameter table queries and sets, use read_param/write_param
// args:
//  i2c_address - i2c address of the device
//  command - the command to give
// return: 0 if successful, see 5.4.10 for error codes
int si115x_issue_command(int i2c_address, char command) {
  char buf;
  // get command count
  si115x_read_reg(i2c_address, RESPONSE0, &buf);
  int comm_count = (buf & 0x0F) + 1; // determine what the next comm count should be
  // issue command
  si115x_write_reg(i2c_address, COMMAND, command);
  // check that command count incremented
  do {
    si115x_read_reg(i2c_address, RESPONSE0, &buf);
    if (buf & 0x10)
      return buf & 0x0F;
  } while ((comm_count != (buf & 0x0F)));
  return 0;
}

// Reads the desired parameter from the parameter table, see 5.3.1
// args:
//  i2c_address - i2c address of the device
//  param - parameter to read, see parameter_t
//  buf - buffer to store value in
// return: 0 if successful, see 5.4.10 for error codes
int si115x_read_param(int i2c_address, parameter_t param, char *buf) {
  int err = si115x_issue_command(i2c_address, PARAM_QUERY(param));
  if (!err)
    si115x_read_reg(i2c_address, RESPONSE1, buf);
  return err;
}

// Writes the given value to the parameter in the parameter table, see 5.3.1
// args:
//  i2c_address - i2c address of the device
//  param - the parameter to write, see parameter_t
//  value - the value of the parameter to write
// return: 0 if successful, see 5.4.10 for error codes
int si115x_write_param(int i2c_address, parameter_t param, char value) {
  si115x_write_reg(i2c_address, HOSTIN0, value);
  return si115x_issue_command(i2c_address, PARAM_SET(param));
}
