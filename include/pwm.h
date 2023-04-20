#pragma once 

#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/irq.h"

#include "../include/pin_info.h"

static uint t = 0;
static uint timer = 0;
static uint val = 0;

void setupPWM(pinInfo* pinout);

void setPWMnoMod(uint16_t* adcData, pinInfo* pinout);

void setPWMMod(uint16_t* adcData, pinInfo* pinout);

void pwm_out();