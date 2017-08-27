#include "nrf_gpio.h"
#include "i5plus.h"

#ifndef ACCEL_H
#define ACCEL_H 1

void accel_init();
void accel_poweroff();
void accel_sethighres();
void accel_setlowres();
int accel_readsamplecount();
void accel_readsample(uint16_t* x, uint16_t* y, uint16_t* z);

#endif
