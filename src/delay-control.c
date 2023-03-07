
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"

/* 
   PIN 15: PWM for controlling the time
   PIN XX: Modulation-Enable Switch
   PIN 21: I2C SDA for the ADC with the Delay-Time, Mod-Freq and Mod-Amp pot
   PIN 22: I2C SCL
*/

static const uint8_t ADR_ADC 0x48; 

void PWMstuff() {
  // // Tell GPIO 0 and 1 they are allocated to the PWM
  // gpio_set_function(15, GPIO_FUNC_PWM);

  // // Find out which PWM slice is connected to GPIO 0 (it's slice 0)
  // uint slice_num = pwm_gpio_to_slice_num(15);

  // pwm_set_clkdiv(slice_num, 125); // pwm clock should now be running at 1MHz

  // // Set period of 4 cycles (0 to 3 inclusive)
  // pwm_set_wrap(slice_num, 3);
  // // Set channel A output high for one cycle before dropping
  // pwm_set_chan_level(slice_num, PWM_CHAN_A, 1);

  // // Set initial B output high for three cycles before dropping
  // pwm_set_chan_level(slice_num, PWM_CHAN_B, 3);

  // // Set the PWM running
  // pwm_set_enabled(slice_num, true);

  // Note we could also use pwm_set_gpio_level(gpio, x) which looks up the
  // correct slice and channel for a given GPIO.
}

int main() {
  stdio_init_all();

  while (true)
  {
    printf("Hello\n");
    sleep_ms(1e3);
  }
  

}