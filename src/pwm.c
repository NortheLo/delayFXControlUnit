#include "../include/pwm.h"

void pwm_out() {
  pwm_clear_irq(pwm_gpio_to_slice_num(0));
  pwm_set_gpio_level(0, val);
  
  timer++;
  if (timer > 30) {
    t++;  
    timer = 0;
  }
}

void setupPWM(pinInfo* pinout) {
  gpio_set_function(pinout->pwm_pin, GPIO_FUNC_PWM);

  pinout->slice = pwm_gpio_to_slice_num(pinout->pwm_pin);

  pwm_clear_irq(pinout->slice);
  pwm_set_irq_enabled(pinout->slice, true);
  irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_out);
  irq_set_enabled(PWM_IRQ_WRAP, true);

  pwm_config config = pwm_get_default_config();

  pwm_config_set_clkdiv(&config, 4.f);
  // Load the configuration into our PWM slice, and set it running.
  pwm_init(pinout->slice, &config, true);

  val = 60e3;
  sleep_ms(300);
}

void setPWMnoMod(uint16_t* adcData, pinInfo* pinout) {
  float d = ((float)(*adcData)/(float) pinout->adc_res) * (float) pinout->maxCounter; // For using the whole value range of the PWM counter
  val = (uint) d;
}

void setPWMMod(uint16_t* adcData, pinInfo* pinout) {

  float a = (float) *(adcData + 2);
  float b = (float) *(adcData + 1);
  float d = ((float)(*adcData) / (float) pinout->adc_res) * (float) pinout->maxCounter; // For using the whole value range of the PWM counter

  float modulation = 4 *  a * sin(b * t) + d;

  // Catch over or underflows 
  if (modulation > (float) pinout->maxCounter) {
    modulation = pinout->maxCounter;
  }
  else if (modulation < 0.0f) {
    modulation = 0;
  }

  printf("Mod Value: %f = %f * sin(2 * %f * %d) + %f\n", modulation, a, b, t, d);
  val = (uint) modulation;
}   