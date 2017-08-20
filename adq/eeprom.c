#include "nrf_gpio.h"
#include "i5plus.h"
#include "twi_master.h"
#include "eeprom.h"


static const twi_master_config_t twi_eeprom = {TWI_Pin_SCL:GPIO_EEPROM_SCL, TWI_Pin_SDA:GPIO_EEPROM_SDA};

void eeprom_init() {
    twi_master_init(twi_eeprom);
}

int eeprom_read_bytes(uint16_t addr, uint8_t *rxbuf, int count) {
    uint8_t txbuf[2];
    txbuf[0] = (addr & 0xff00) >> 8;
    txbuf[1] = addr & 0xff;
    if (!twi_master_transfer(twi_eeprom, I2C_EEPROM, txbuf, 2, false)) {
        return false;
    }

    return twi_master_transfer(twi_eeprom, I2C_EEPROM | TWI_READ_BIT, rxbuf, count, true);
}

int eeprom_read_byte(uint16_t addr) {
    uint8_t rxbuf[1];
    if (!eeprom_read_bytes(addr, rxbuf, 1)) {
        return -1;
    }
    return rxbuf[0];
}
