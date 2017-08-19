#include "nrf_gpio.h"
#include "i5plus.h"

#ifndef EEPROM_H
#define EEPROM_H 1

void eeprom_init();
int eeprom_read_bytes(uint16_t addr, uint8_t *rxbuf, int count);
int eeprom_read_byte(uint16_t addr);

#endif
