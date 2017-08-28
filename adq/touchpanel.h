#include "nrf_gpio.h"
#include "i5plus.h"

#ifndef TOUCHPANEL_H
#define TOUCHPANEL_H 1

void touchpanel_init();
uint8_t touchpanel_read();

#endif
