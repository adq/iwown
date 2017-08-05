#include "nrf_gpio.h"
#include "i5plus.h"

void motorInit() {
  nrf_gpio_cfg_output(GPIO_MOTOR);
}

void motorOn() {
  nrf_gpio_pin_set(GPIO_MOTOR);
}

void motorOff() {
  nrf_gpio_pin_clear(GPIO_MOTOR);
}
