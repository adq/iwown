#include "nrf_gpio.h"
#include "i5plus.h"

static void __INLINE ledInit() {
  nrf_gpio_cfg_output(GPIO_LED);
}

static void __INLINE ledOn() {
  nrf_gpio_pin_set(GPIO_LED);
}

static void __INLINE ledOff() {
  nrf_gpio_pin_clear(GPIO_LED);
}
