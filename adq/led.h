#include "nrf_gpio.h"
#include "i5plus.h"

static void __INLINE ledInit() {
  nrf_gpio_cfg_output(LED_PIN);
}

static void __INLINE ledOn() {
  nrf_gpio_pin_set(LED_PIN);
}

static void __INLINE ledOff() {
  nrf_gpio_pin_clear(LED_PIN);
}
