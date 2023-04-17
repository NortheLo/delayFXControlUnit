#pragma once

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "../include/adc.h"

void setupGPIO(adcinfo* pot);