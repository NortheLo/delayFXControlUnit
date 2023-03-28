#include <stdio.h>
#include <time.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/irq.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

/* 
   PIN 13: Modulation-Enable Switch
   PIN 15: PWM for controlling the time
   PIN 26: Delay-Time potentiometer 
   PIN 27: Mod-Freq potentiometer 
   PIN 28: Mod-Amp potentiometer
*/

#define MOD_SWT   13    // Swtch for toggling modulation
#define PWM_PIN   0
#define DTIM_PIN  26    // Mod OFF: Delay Time; MOD ON: Timeoffset in modulation
#define MFRQ_PIN  27    // MOD ON: Frequency of modulation
#define MAMP_PIN  28    // MOD ON: Amplitude of modulation

static const uint adc_res = 4096;
static const uint maxCounter = 0xffff;

typedef struct {
  uint slice;
  bool enabled;
  uint val;
} pwmdata;

static pwmdata modPWM;

clock_t getClock() {
  return (clock_t) time_us_64() / 1e4;
}

void pwm_out() {
  pwm_clear_irq(pwm_gpio_to_slice_num(PWM_PIN));

  if (modPWM.enabled) {
      pwm_set_gpio_level(PWM_PIN, modPWM.val);
  }
  
  else if (!modPWM.enabled) {
      pwm_set_gpio_level(PWM_PIN, modPWM.val);
  }
}

void setupPWM() {
  gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);

  modPWM.slice = pwm_gpio_to_slice_num(PWM_PIN);

  pwm_clear_irq(modPWM.slice);
  pwm_set_irq_enabled(modPWM.slice, true);
  irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_out);
  irq_set_enabled(PWM_IRQ_WRAP, true);

  pwm_config config = pwm_get_default_config();

  pwm_config_set_clkdiv(&config, 4.f);
  // Load the configuration into our PWM slice, and set it running.
  pwm_init(modPWM.slice, &config, true);

  modPWM.enabled = false;
  modPWM.val = 60e3;
}

void setPWMnoMod(uint16_t* adcData) {
  float d = ((float)(*adcData)/(float)adc_res) * (float)maxCounter; // For using the whole value range of the PWM counter
  modPWM.val = (uint) d;
}

void setPWMMod(uint16_t* adcData) {
  float a = (float) *(adcData + 2);
  float b = (float) *(adcData + 1);
  float d = ((float)(*adcData)/(float)adc_res) * (float)maxCounter; // For using the whole value range of the PWM counter
  clock_t t1 = getClock();

  float modulation = 5 *  a * sin(10 * b * t1) + d; // Range the period should be between ~0.05Hz and 50Hz; b therefore must be in the range of 7.2 to 7.2e3 (2*adcRes)
  //printf("Mod Value: %f = %f * sin(2 * %f * t) + %f\n", modulation, a, b, d);
  //printf("Mod Val %d\n", (uint) modulation);
  // Catch over or underflows 
  if (modulation > (float)maxCounter) {
    modulation = maxCounter;
  }
  else if (modulation < 0.0f) {
    modulation = 0;
  }

  modPWM.val = (uint) modulation;
}

void setupADC() {
  adc_init();
  adc_gpio_init(DTIM_PIN);
  adc_gpio_init(MFRQ_PIN);
  adc_gpio_init(MAMP_PIN);
}

void loopADC(uint16_t* adcData) {
  int cnt = 0;
  adc_select_input(cnt);

  while (true) {
    *(adcData+cnt) = adc_read();
    modPWM.enabled = gpio_get(MOD_SWT);
    
    if (modPWM.enabled) {
      gpio_put(PICO_DEFAULT_LED_PIN, 1);
      setPWMMod(adcData);
    }

    else {
      gpio_put(PICO_DEFAULT_LED_PIN, 0);
      setPWMnoMod(adcData);
    }

    cnt++;
    if (cnt > 2) {
      cnt = 0;
    }
    adc_select_input(cnt);
    //sleep_ms(100);
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

  stdio_init_all();
  setupPWM();
  setupGPIO();
  setupADC();
  loopADC(dat);
}