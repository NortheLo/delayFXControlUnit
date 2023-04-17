#pragma once 

#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/irq.h"

static uint t = 0;
static uint val = 0;

typedef struct {
  uint pin;
  uint slice;
  uint adc_res;
  uint maxCounter;
} pwmdata;

void setupPWM(pwmdata* infoPWM);

void setPWMnoMod(uint16_t* adcData, pwmdata* infoPWM);

void setPWMMod(uint16_t* adcData, pwmdata* infoPWM);

void pwm_out();