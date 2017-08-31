#include "nrf_gpio.h"
#include "i5plus.h"

#ifndef POWER_H
#define POWER_H 1

extern uint32_t battery_percent;
void power_init();
void power_dosample();
uint8_t power_readusbstatus();

#endif
