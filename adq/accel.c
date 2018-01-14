#include "nrf_gpio.h"
#include "i5plus.h"
#include "twi_master.h"
#include "accel.h"
#include "simple_uart.h"
#include <stdio.h>

static twi_master_config_t twi_accelerometer = {TWI_Pin_SCL:GPIO_ACCEL_SCL, TWI_Pin_SDA:GPIO_ACCEL_SDA};

void accel_init() {
    uint8_t txbuf[2];

    twi_master_init(twi_accelerometer);

    // enable XYZ + 50Hz mode
    txbuf[0] = 0x20;
    txbuf[1] = 0x47;
    twi_master_transfer(twi_accelerometer, I2C_ACCEL, txbuf, 2, true);

    // HR mode on, other defaults
    txbuf[0] = 0x23;
    txbuf[1] = 0x08;
    twi_master_transfer(twi_accelerometer, I2C_ACCEL, txbuf, 2, true);

    // disable interrupts
    txbuf[0] = 0x25;
    txbuf[1] = 0x00;
    twi_master_transfer(twi_accelerometer, I2C_ACCEL, txbuf, 2, true);
}

void accel_poweroff() {
    uint8_t txbuf[2];

    txbuf[0] = 0x20;
    txbuf[1] = 0x80;
    twi_master_transfer(twi_accelerometer, I2C_ACCEL, txbuf, 2, true);
}

void accel_setfifohighres() {
    uint8_t txbuf[2];

    // enable XYZ + 50Hz mode
    txbuf[0] = 0x20;
    txbuf[1] = 0x47;
    twi_master_transfer(twi_accelerometer, I2C_ACCEL, txbuf, 2, true);

    // enable FIFO, others the same
    txbuf[0] = 0x24;
    txbuf[1] = 0x40;
    twi_master_transfer(twi_accelerometer, I2C_ACCEL, txbuf, 2, true);

    // FIFO ctrl: stream mode, 25 records
    txbuf[0] = 0x2E;
    txbuf[1] = 0x99;
    twi_master_transfer(twi_accelerometer, I2C_ACCEL, txbuf, 2, true);
}

void accel_setfifolowres() {
    uint8_t txbuf[2];

    // enable XYZ + 10Hz mode
    txbuf[0] = 0x20;
    txbuf[1] = 0x27;
    twi_master_transfer(twi_accelerometer, I2C_ACCEL, txbuf, 2, true);

    // enable FIFO, others the same
    txbuf[0] = 0x24;
    txbuf[1] = 0x40;
    twi_master_transfer(twi_accelerometer, I2C_ACCEL, txbuf, 2, true);

    // FIFO ctrl: stream mode, 25 records
    txbuf[0] = 0x2E;
    txbuf[1] = 0x9F;
    twi_master_transfer(twi_accelerometer, I2C_ACCEL, txbuf, 2, true);
}

int accel_readsamplecount() {
    uint8_t txbuf[1];

    txbuf[0] = 0x2F;
    twi_master_transfer(twi_accelerometer, I2C_ACCEL, txbuf, 1, false);
    twi_master_transfer(twi_accelerometer, I2C_ACCEL | TWI_READ_BIT, txbuf, 1, true);

    return txbuf[0] & 0x1f;
}

void accel_readsample(int16_t* x, int16_t* y, int16_t* z) {
    uint8_t txbuf[6];

    txbuf[0] = 0xA8; // top bit set => multibyte
    twi_master_transfer(twi_accelerometer, I2C_ACCEL, txbuf, 1, false);
    twi_master_transfer(twi_accelerometer, I2C_ACCEL | TWI_READ_BIT, txbuf, 6, true);

    *x = *((int16_t*) (txbuf + 0));
    *y = *((int16_t*) (txbuf + 2));
    *z = *((int16_t*) (txbuf + 4));
}
