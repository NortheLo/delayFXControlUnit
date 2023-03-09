
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#include "hardware/adc.h"
#include "hardware/gpio.h"

/* 
   PIN 16: PWM for controlling the time
   PIN XX: Modulation-Enable Switch
   PIN 26: Delay-Time potentiometer 
   PIN 27: Mod-Freq potentiometer 
   PIN 28: Mod-Amp potentiometer
*/

#define PWM_PIN   16
#define DTIM_PIN  26
#define MFRQ_PIN  27
#define MAMP_PIN  28

void PWMstuff() {
  gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);

  uint slice_num = pwm_gpio_to_slice_num(PWM_PIN);

  pwm_set_clkdiv(slice_num, 125); //PWM clock should now be running at (PICO_CLOCK=125MHz/125) = 1MHz

  // Set the PWM running
  pwm_set_enabled(slice_num, true);

  pwm_set_wrap(slice_num, 1e3);
  // Set channel A output high for one cycle before dropping
  pwm_set_chan_level(slice_num, PWM_CHAN_A, 500);
}

void setupADC() {
  adc_init();
  adc_gpio_init(DTIM_PIN);
  adc_gpio_init(MFRQ_PIN);
  adc_gpio_init(MAMP_PIN);
}

void adcLoop(uint16_t* adcData) {
  int cnt = 0;
  adc_select_input(cnt);

  while (true)
  {
    *(adcData+cnt) = adc_read();

    //printf("Ch %d: %u\n", cnt, *(adcData + cnt));
    cnt++;
    if (cnt > 2) {
      cnt = 0;
    }
    adc_select_input(cnt);
    
    sleep_ms(100);
  }
}

int main() {
  uint16_t dat[2] = {0};
  stdio_init_all();
  setupADC();

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
  gpio_put(PICO_DEFAULT_LED_PIN, 1);

  adcLoop(dat);
  gpio_put(PICO_DEFAULT_LED_PIN, 0);
}