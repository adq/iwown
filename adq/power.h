#include "nrf_gpio.h"
#include "i5plus.h"

#ifndef POWER_H
#define POWER_H 1

void power_init();
uint8_t power_readusbstatus();

#endif
