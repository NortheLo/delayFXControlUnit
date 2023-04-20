#pragma once 

#include "pico/stdlib.h"
#include "hardware/adc.h"

#include "../include/pin_info.h"
#include "../include/pwm.h"

void setupADC(pinInfo* pot);

void loopADC(uint16_t* adcData, pinInfo* pinout);