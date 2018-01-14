#include "nrf_gpio.h"
#include "i5plus.h"

#ifndef ACCEL_H
#define ACCEL_H 1

void accel_init();
void accel_poweroff();
void accel_sethighres();
void accel_setlowres();
int accel_readsamplecount();
void accel_readsample(int16_t* x, int16_t* y, int16_t* z);

#endif
