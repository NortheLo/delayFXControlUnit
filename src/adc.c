#include "../include/adc.h"


void setupADC(adcinfo* pot) {
  adc_init();
  adc_gpio_init(pot->tim_pot);
  adc_gpio_init(pot->frq_pot);
  adc_gpio_init(pot->amp_pot);
}

void loopADC(uint16_t* adcData, adcinfo* pot, pwmdata* infoPWM) {
  int cnt = 0;
  bool enabled = false;
  adc_select_input(cnt);

  while (true) {
    *(adcData+cnt) = adc_read();
    enabled = gpio_get(pot->swt);
    
    if (enabled) {
      gpio_put(PICO_DEFAULT_LED_PIN, 1);
      setPWMMod(adcData, infoPWM);
    }

    else {
      gpio_put(PICO_DEFAULT_LED_PIN, 0);
      setPWMnoMod(adcData, infoPWM);
    }

    cnt++;
    if (cnt > 2) {
      cnt = 0;
    }
    adc_select_input(cnt);
    sleep_ms(15);
  }
}