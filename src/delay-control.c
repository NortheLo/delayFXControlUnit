
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"

#include "../libs/ads1115.h"

/* 
   PIN 15: PWM for controlling the time
   PIN XX: Modulation-Enable Switch
   PIN 20: I2C SDA for the ADC with the Delay-Time, Mod-Freq and Mod-Amp pot
   PIN 21: I2C SCL
*/
#define I2C_FREQ 40e3
#define I2C_PORT i2c1
static const uint8_t ADR_ADC = 0x48; 
static const uint8_t SDA_PIN = 14;
static const uint8_t SCL_PIN = 15;

struct ads1115_adc adc;

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


    // Initialise I2C
    i2c_init(I2C_PORT, I2C_FREQ);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    // Initialise ADC
    ads1115_init(I2C_PORT, ADR_ADC, &adc);
    
    // and the full-scale voltage range is set to +/- 4.096 V.
    ads1115_set_input_mux(ADS1115_MUX_SINGLE_1, &adc);
    ads1115_set_pga(ADS1115_PGA_4_096, &adc);
    ads1115_set_data_rate(ADS1115_RATE_475_SPS, &adc);

    // Write the configuration for this to have an effect.
    ads1115_write_config(&adc);

    // Data containers
    float volts;
    uint16_t adc_value;

    while (true) {
        ads1115_set_input_mux(ADS1115_MUX_SINGLE_0, &adc);
        // Read a value, convert to volts, and print.
        ads1115_read_adc(&adc_value, &adc);
        volts = ads1115_raw_to_volts(adc_value, &adc);
        printf("ADC Ch-0: %u  Voltage: %f\n", adc_value, volts);

        ads1115_set_input_mux(ADS1115_MUX_SINGLE_1, &adc);
        // Read a value, convert to volts, and print.
        ads1115_read_adc(&adc_value, &adc);
        volts = ads1115_raw_to_volts(adc_value, &adc);
        printf("ADC Ch-1: %u  Voltage: %f\n", adc_value, volts);

        ads1115_set_input_mux(ADS1115_MUX_SINGLE_2, &adc);
        // Read a value, convert to volts, and print.
        ads1115_read_adc(&adc_value, &adc);
        volts = ads1115_raw_to_volts(adc_value, &adc);
        printf("ADC Ch-2: %u  Voltage: %f\n", adc_value, volts);

        sleep_ms(10);
    }

}