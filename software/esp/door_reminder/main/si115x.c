#include "si115x.h"

// I2C Registers
#define PART_ID 0x0
#define REV_ID 0x01
#define MFR_ID 0x02
#define INFO0 0x03
#define INFO1 0x04
#define HOSTIN3 0x07
#define HOSTIN2 0x08
#define HOSTIN1 0x09
#define HOSTIN0 0x0A
#define COMMAND 0x0B
#define IRQ_ENABLE 0x0F
#define RESPONSE1 0x10
#define RESPONSE0 0x11
#define IRQ_STATUS 0x12
#define HOSTOUT0 0x13
#define HOSTOUT1 0x14
#define HOSTOUT2 0x15
#define HOSTOUT3 0x16
#define HOSTOUT4 0x17
#define HOSTOUT5 0x18
#define HOSTOUT6 0x19
#define HOSTOUT7 0x1A
#define HOSTOUT8 0x1B
#define HOSTOUT9 0x1C
#define HOSTOUT10 0x1D
#define HOSTOUT11 0x1E
#define HOSTOUT12 0x1F
#define HOSTOUT13 0x20
#define HOSTOUT14 0x21
#define HOSTOUT15 0x22
#define HOSTOUT16 0x23
#define HOSTOUT17 0x24
#define HOSTOUT18 0x25
#define HOSTOUT19 0x26
#define HOSTOUT20 0x27
#define HOSTOUT21 0x28
#define HOSTOUT22 0x29
#define HOSTOUT23 0x2A
#define HOSTOUT24 0x2B
#define HOSTOUT25 0x2C

// Command Register Commands
#define RESET_CMD_CTR 0x00
#define RESET_SW 0x01
#define FORCE 0x11
#define PAUSE 0x12
#define START 0x13
#define PARAM_QUERY(param) (0b01000000 | param)
#define PARAM_SET(param) (0b10000000 | param) // Value goes into INPUT0

// Parameter table
typedef enum {
  // I2C Address (Temp)
  I2C_ADDR = 0x00,
  // Channel List
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
  // MEASURE RATE
  MEASRATE_H = 0x1A,
  MEASRATE_L = 0x1B,
  // MEASCOUNT
  MEASCOUNT1 = 0x1C,
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
} parameters_t;

enum ADCMUX {
  SMALL_IR = 0b00000,
  MED_IR = 0b00001,
  LRG_IR = 0b00010,
  WHITE = 0b01011,
  LRG_WHITE = 0b01101
};

// 7.2.1
#define _DECIM_RATE(n) (n << 4)
#define ADCCONFIG(decim_rate, adcmux) (_DECIM_RATE(decim_rate) | adcmux)

// 7.2.2
enum HSIG { NORMAL = 0b0, HIGH = 0b1 };
#define ADCSENS(hsig, sw_gain, hw_gain) (hsig << 7 | sw_gain << 4 | hw_gain)

// 7.2.3
#define ADCPOST(rsrvd, bit_out_24, post_shift, thresh_pol, thresh_sel)                             \
  (rsrvd << 7 | bit_out_24 << 6 | post_shift << 3 | thresh_pol << 2 | thresh_sel)

// 7.2.4
#define MEASCONFIG(counter_index, led_trim, bank_sel, led2_en, led3_en, led1_en)                   \
  (counter_index << 6 | led_trim << 4 | bank_sel << 3 | led2_en << 2 | led3_en << 1 | led1_en)

// Verify each command with a read to RESPONSE0