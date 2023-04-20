#pragma once

#include "pico/stdlib.h"

typedef struct {
    /* Used pins */
    uint tim_pot;
    uint frq_pot;
    uint amp_pot;
    uint mod_swt;
    uint pwm_pin;
    
    /* Info for using the PWM functionality */
    uint slice;
    uint adc_res;
    uint maxCounter;
} pinInfo;