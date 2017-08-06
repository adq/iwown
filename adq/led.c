#include "nrf_gpio.h"
#include "i5plus.h"

void led_init() {
  nrf_gpio_cfg_output(GPIO_LED);
}

void led_on() {
  nrf_gpio_pin_set(GPIO_LED);
}

void led_off() {
  nrf_gpio_pin_clear(GPIO_LED);
}
