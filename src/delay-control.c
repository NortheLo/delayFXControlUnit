#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#include "hardware/adc.h"
#include "hardware/gpio.h"

/* 
   PIN 13: Modulation-Enable Switch
   PIN 15: PWM for controlling the time
   PIN 26: Delay-Time potentiometer 
   PIN 27: Mod-Freq potentiometer 
   PIN 28: Mod-Amp potentiometer
*/

#define MOD_SWT   13
#define PWM_PIN   15
#define DTIM_PIN  26
#define MFRQ_PIN  27
#define MAMP_PIN  28

static const uint maxCounter = 10e3;
static const uint adc_res = 4096;

void setupPWM(uint slice_num) {
  gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);

  slice_num = pwm_gpio_to_slice_num(PWM_PIN);

  pwm_set_clkdiv(slice_num, 125); //PWM clock should now be running at (PICO_CLOCK=125MHz/125) = 1MHz

  // Set the PWM running
  pwm_set_enabled(slice_num, true);

  pwm_set_wrap(slice_num, maxCounter);
  // Set channel A output high for one cycle before dropping
  pwm_set_chan_level(slice_num, PWM_CHAN_A, 5e3);

  sleep_ms(1e3); 
}

void setPWMnoMod(uint16_t* adcData, uint slice_num) {
  float t = ((float)(*adcData)/(float)adc_res) * (float)maxCounter;
  pwm_set_chan_level(slice_num, PWM_CHAN_A, (uint)t);
}

void setPWMMod(uint16_t* adcData, uint slice_num) {
  float x = 0.5 * M_PI;
  float t = *(adcData + 3) sin( *(adcData + 2) * x);
  printf("Sin: %f\n", t);

  float d = ((float)(*adcData)/(float)adc_res) * (float)maxCounter;

  //   pwm_set_chan_level(slice_num, PWM_CHAN_A, (uint) (t + d));
}

void setupADC() {
  adc_init();
  adc_gpio_init(DTIM_PIN);
  adc_gpio_init(MFRQ_PIN);
  adc_gpio_init(MAMP_PIN);
}

void loopADC(uint16_t* adcData, uint slice) {
  int cnt = 0;
  adc_select_input(cnt);

  while (true)
  {
    *(adcData+cnt) = adc_read();

    //printf("Ch %d: %u\n", cnt, *(adcData + cnt));
    cnt++;
    if (cnt > 2) {
      cnt = 0;

      bool mod = gpio_get(MOD_SWT);
      if (mod == true) {
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        setPWMMod(adcData, slice);
      }

      else {
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        setPWMnoMod(adcData, slice);
      }
      
    }
    adc_select_input(cnt);
    
    sleep_ms(100);
  }
}

void setupGPIO() {
  gpio_init(MOD_SWT);
  gpio_set_dir(MOD_SWT, GPIO_IN);

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

  gpio_put(PICO_DEFAULT_LED_PIN, 1);
  sleep_ms(1e3);
  gpio_put(PICO_DEFAULT_LED_PIN, 0);
}

int main() {
  uint16_t dat[2] = {0};
  uint slicePWM;

  stdio_init_all();
  setupPWM(slicePWM);
  setupGPIO();
  setupADC();
  loopADC(dat, slicePWM);
}