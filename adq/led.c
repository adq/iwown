#include "nrf_gpio.h"
#include "i5plus.h"

void ledInit() {
  nrf_gpio_cfg_output(GPIO_LED);
}

void ledOn() {
  nrf_gpio_pin_set(GPIO_LED);
}

void ledOff() {
  nrf_gpio_pin_clear(GPIO_LED);
}
