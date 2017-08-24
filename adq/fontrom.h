#include "nrf_gpio.h"
#include "i5plus.h"

#ifndef FONTROM_H
#define FONTROM_H 1

void fontrom_init();

uint32_t fontrom_read_bytes(uint32_t addr, uint8_t *rxbuf, uint16_t rxcount);

#endif
