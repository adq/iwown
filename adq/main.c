/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "i5plus.h"
#include "i2c.h"
#include "led.h"

i2cBus oled;
i2cBus accelerometer;
i2cBus eeprom;

int main(void)
{
    ledInit();
    i2cInit(&oled, GPIO_OLED_SCL, GPIO_OLED_SDA, I2C_BITRATE);
    i2cInit(&accelerometer, GPIO_ACCEL_SCL, GPIO_ACCEL_SDA, I2C_BITRATE);
    i2cInit(&eeprom, GPIO_EEPROM_SCL, GPIO_EEPROM_SDA, I2C_BITRATE);

    while (true)
    {
        ledOn();
        nrf_delay_ms(500);
        ledOff();
        nrf_delay_ms(500);
    }
}
