#pragma once 

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "../include/pwm.h"

typedef struct adc
{
    uint tim_pot;
    uint frq_pot;
    uint amp_pot;
    uint swt;
} adcinfo;

void setupADC(adcinfo* pot);

void loopADC(uint16_t* adcData, adcinfo* pot, pwmdata* infoPWM);