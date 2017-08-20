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
#include "led.h"
#include "ble.h"
#include "eeprom.h"
#include "motor.h"
#include "app_timer.h"
#include "app_gpiote.h"
#include "app_scheduler.h"
#include "softdevice_handler.h"
#include "twi_master.h"
#include "simple_uart.h"
#include "bootloader_types.h"


// NOTE: get into DFU bootloader: NRF_POWER->GPREGRET == BOOTLOADER_DFU_START  then soft reset
// sd_power_gpregret_set(BOOTLOADER_DFU_START);
// sd_nvic_SystemReset();
// use dfu_app_handler

twi_master_config_t twi_accelerometer = {TWI_Pin_SCL:GPIO_ACCEL_SCL, TWI_Pin_SDA:GPIO_ACCEL_SDA};
twi_master_config_t twi_oled = {TWI_Pin_SCL:GPIO_OLED_SCL, TWI_Pin_SDA:GPIO_OLED_SDA};

static void softdevice_init(void)
{
    SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_XTAL_20_PPM, true);
}

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
    motor_init();
    led_init();

    softdevice_init();
    timers_init();
    gpiote_init();
    scheduler_init();
    ble_stack_init();
    simple_uart_config(0, GPIO_UART_TX, 0, GPIO_UART_RX, false);
    eeprom_init();
    twi_master_init(twi_accelerometer);
    twi_master_init(twi_oled);

    advertising_start();
    // led_on();

    // uint8_t buf[10];
    // buf[0] = 0x0d;
    // twi_master_transfer(accelerometer, 0x3a, buf, 1, false);
    // int ok = twi_master_transfer(accelerometer, 0x3a | TWI_READ_BIT, buf, 1, true);

    // uint8_t strbuf[50];
    // sprintf((char*) strbuf, "ACCEL %i %x\r\n", ok, buf[0]);
    simple_uart_putstring((uint8_t*) "HELLO\r\n");

    led_on();

    while (true)
    {
        app_sched_execute();
        power_manage();
    }
}
