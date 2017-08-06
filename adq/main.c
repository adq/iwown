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
#include <string.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "i5plus.h"
#include "i2c.h"
#include "led.h"
#include "ble.h"
#include "motor.h"
#include "app_timer.h"
#include "app_gpiote.h"
#include "app_scheduler.h"
#include "softdevice_handler.h"



i2cBus oled;
i2cBus accelerometer;
i2cBus eeprom;


static void gpiote_init(void)
{
    APP_GPIOTE_INIT(APP_GPIOTE_MAX_USERS);
}

static void timers_init(void)
{
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS, APP_TIMER_OP_QUEUE_SIZE, true);
}

static void scheduler_init(void)
{
    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}

static void power_manage(void)
{
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}

int main(void)
{
    ledInit();
    #ifdef MICROBIT
    nrf_gpio_cfg_output(4);
    nrf_gpio_pin_clear(4);
    nrf_gpio_cfg_output(13);
    #endif

    timers_init();
    gpiote_init();
    ble_stack_init();
    scheduler_init();
    gap_params_init();
    advertising_init();

    // motorInit();
    // i2cInit(&oled, GPIO_OLED_SCL, GPIO_OLED_SDA, I2C_BITRATE);
    // i2cInit(&accelerometer, GPIO_ACCEL_SCL, GPIO_ACCEL_SDA, I2C_BITRATE);
    // i2cInit(&eeprom, GPIO_EEPROM_SCL, GPIO_EEPROM_SDA, I2C_BITRATE);

    advertising_start();
    ledOn();
    #ifdef MICROBIT
    nrf_gpio_pin_set(13);
    #endif

    while (true)
    {
        app_sched_execute();
        power_manage();
    }
}
