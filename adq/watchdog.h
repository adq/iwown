#include "nrf_gpio.h"
#include "i5plus.h"

#ifndef WATCHDOG_H
#define WATCHDOG_H 1

void watchdog_init();
void watchdog_reload();

#endif
