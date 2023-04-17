#include "../include/pwm.h"

void pwm_out() {
  pwm_clear_irq(pwm_gpio_to_slice_num(0));
  pwm_set_gpio_level(0, val);
  t++;
}

void setupPWM(pwmdata* infoPWM) {
  gpio_set_function(infoPWM->pin, GPIO_FUNC_PWM);

  infoPWM->slice = pwm_gpio_to_slice_num(infoPWM->pin);

  pwm_clear_irq(infoPWM->slice);
  pwm_set_irq_enabled(infoPWM->slice, true);
  irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_out);
  irq_set_enabled(PWM_IRQ_WRAP, true);

  pwm_config config = pwm_get_default_config();

  pwm_config_set_clkdiv(&config, 4.f);
  // Load the configuration into our PWM slice, and set it running.
  pwm_init(infoPWM->slice, &config, true);

  val = 60e3;
  sleep_ms(300);
}

void setPWMnoMod(uint16_t* adcData, pwmdata* infoPWM) {
  float d = ((float)(*adcData)/(float) infoPWM->adc_res) * (float) infoPWM->maxCounter; // For using the whole value range of the PWM counter
  val = (uint) d;
}

void setPWMMod(uint16_t* adcData, pwmdata* infoPWM) {
  float a = (float) *(adcData + 2);
  float b = (float) *(adcData + 1);
  float d = ((float)(*adcData) / (float) infoPWM->adc_res) * (float) infoPWM->maxCounter; // For using the whole value range of the PWM counter

  float modulation = 4 *  a * sin(b * t) + d;
  
  // Catch over or underflows 
  if (modulation > (float) infoPWM->maxCounter) {
    modulation = infoPWM->maxCounter;
  }
  else if (modulation < 0.0f) {
    modulation = 0;
  }

  //printf("Mod Value: %f = %f * sin(2 * %f * t) + %f\n", modulation, a, b, d);
  val = (uint) modulation;
}   