#include <stdio.h>
#include <time.h>
#include "pico/stdlib.h"

#include "../include/pin_info.h"
#include "../include/pwm.h"
#include "../include/adc.h"
#include "../include/gpio.h"

#define MOD_SWT   13    // Swtch for toggling modulation
#define PWM_PIN   0     // PWM pin 
#define DTIM_PIN  26    // Mod OFF: Delay Time; MOD ON: Timeoffset in modulation
#define MFRQ_PIN  27    // MOD ON: Frequency of modulation
#define MAMP_PIN  28    // MOD ON: Amplitude of modulation

#define ADC_RES           4096  // Only true for picos 12 Bit ADC
#define MAX_COUNTER_VALUE 0xffff 

int main() {
  uint16_t adcValues[2] = {0};

  pinInfo pico_pinout = {DTIM_PIN, MFRQ_PIN, MAMP_PIN, 
                         MOD_SWT, PWM_PIN, 0, ADC_RES, 
                         MAX_COUNTER_VALUE};

  stdio_init_all();
  setupPWM(&pico_pinout);
  setupGPIO(&pico_pinout);
  setupADC(&pico_pinout);

  loopADC(adcValues, &pico_pinout);
}