#include "../include/gpio.h"

void setupGPIO(pinInfo* pinout) {
  gpio_init(pinout->mod_swt);
  gpio_set_dir(pinout->mod_swt, GPIO_IN);

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

  gpio_put(PICO_DEFAULT_LED_PIN, 1);
  sleep_ms(1e3);
  gpio_put(PICO_DEFAULT_LED_PIN, 0);
}