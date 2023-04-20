#pragma once

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "../include/pin_info.h"
#include "../include/adc.h"

void setupGPIO(pinInfo* pinout);