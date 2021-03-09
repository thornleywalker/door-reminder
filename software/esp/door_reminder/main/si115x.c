#include "si115x.h"

typedef enum {
  I2C_ADDR = 0x00,
  CHAN_LIST,
  ADCCONFIG0,
  ADCSENS0,
  ADCPOST0,
  MEASCONFIG0,
  ADCCONFIG1,
  ADCSENS1,
  ADCPOST1,
  MEASCONFIG1,
  ADCCONFIG2,
  ADCSENS2,
  ADCPOST2,
  MEASCONFIG2,
  ADCCONFIG3,
  ADCSENS3,
  ADCPOST3,
  MEASCONFIG3,
  ADCCONFIG4,
  ADCSENS4,
  ADCPOST4,
  MEASCONFIG4,
  ADCCONFIG5,
  ADCSENS5,
  ADCPOST5,
  MEASCONFIG5,
  MEASRATE_H,
  MEASRATE_L,
  MEASCOUNT1,
  MEASCOUNT2,
  MEASCOUNT3,
  LED1_A,
  LED1_B,
  LED3_A,
  LED3_B,
  LED2_A,
  LED2_B,
  THRESHOLD0_H,
  THRESHOLD0_L,
  THRESHOLD1_H,
  THRESHOLD1_L,
  UPPER_THRESHOLD_H,
  UPPER_THRESHOLD_L,
  BURST,
  LOWER_THRESHOLD_H,
  LOWER_THRESHOLD_L
} parameters_t;

enum ADCMUX {
  SMALL_IR = 0b00000,
  MED_IR = 0b00001,
  LRG_IR = 0b00010,
  WHITE = 0b01011,
  LRG_WHITE = 0b01101
};
#define _DECIM_RATE(n) (n << 4)
#define ADCCONFIG(decim_rate, adcmux) (_DECIM_RATE(decim_rate) | adcmux)

enum HSIG { NORMAL = 0b0, HIGH = 0b1 };
#define ADCSENS(hsig, sw_gain, hw_gain) (hsig << 7 |)