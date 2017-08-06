#include "nrf_gpio.h"
#include "i5plus.h"

void motor_init() {
  nrf_gpio_cfg_output(GPIO_MOTOR);
}

void motor_on() {
  nrf_gpio_pin_set(GPIO_MOTOR);
}

void motor_off() {
  nrf_gpio_pin_clear(GPIO_MOTOR);
}
