#include "nrf_gpio.h"
#include "i5plus.h"

void led_init() {
  nrf_gpio_pin_set(GPIO_LED);
  NRF_GPIO->PIN_CNF[GPIO_LED] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) \
                                 |(GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)    \
                                 |(GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)  \
                                 |(GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) \
                                 |(GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
}

void led_on() {
  nrf_gpio_pin_clear(GPIO_LED);
}

void led_off() {
  nrf_gpio_pin_set(GPIO_LED);
}
