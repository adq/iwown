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
#include "motor.h"
#include "app_timer.h"
#include "app_gpiote.h"
#include "app_scheduler.h"
#include "softdevice_handler.h"
#include "twi_master.h"
#include "simple_uart.h"


twi_master_config_t accelerometer = {TWI_Pin_SCL:0, TWI_Pin_SDA:30};

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

void simple_uart_config2(uint8_t rts_pin_number,
                        uint8_t txd_pin_number,
                        uint8_t cts_pin_number,
                        uint8_t rxd_pin_number,
                        bool    hwfc)
{
/** @snippet [Configure UART RX and TX pin] */
    nrf_gpio_cfg_output(txd_pin_number);
    nrf_gpio_cfg_input(rxd_pin_number, NRF_GPIO_PIN_NOPULL);

    NRF_UART0->PSELTXD = txd_pin_number;
    NRF_UART0->PSELRXD = rxd_pin_number;
/** @snippet [Configure UART RX and TX pin] */
    if (hwfc)
    {
        nrf_gpio_cfg_output(rts_pin_number);
        nrf_gpio_cfg_input(cts_pin_number, NRF_GPIO_PIN_NOPULL);
        NRF_UART0->PSELCTS = cts_pin_number;
        NRF_UART0->PSELRTS = rts_pin_number;
        NRF_UART0->CONFIG  = (UART_CONFIG_HWFC_Enabled << UART_CONFIG_HWFC_Pos);
    }

    NRF_UART0->BAUDRATE      = (UART_BAUDRATE_BAUDRATE_Baud115200 << UART_BAUDRATE_BAUDRATE_Pos);
    NRF_UART0->ENABLE        = (UART_ENABLE_ENABLE_Enabled << UART_ENABLE_ENABLE_Pos);
    NRF_UART0->TASKS_STARTTX = 1;
    NRF_UART0->TASKS_STARTRX = 1;
    NRF_UART0->EVENTS_RXDRDY = 0;
}

int main(void)
{
    led_init();
    timers_init();
    gpiote_init();
    scheduler_init();
    ble_stack_init();
    // motor_init();

    #ifdef MICROBIT
    nrf_gpio_cfg_output(4);
    nrf_gpio_pin_clear(4);
    nrf_gpio_cfg_output(13);
    simple_uart_config2(0, 24, 0, 25, false);
    twi_master_init(accelerometer);
    #endif

    advertising_start();
    led_on();
    #ifdef MICROBIT
    nrf_gpio_pin_set(13);
    #endif

    uint8_t buf[10];
    buf[0] = 0x0d;
    twi_master_transfer(accelerometer, 0x3a, buf, 1, false);
    int ok = twi_master_transfer(accelerometer, 0x3a | TWI_READ_BIT, buf, 1, true);

    uint8_t strbuf[50];
    sprintf((char*) strbuf, "ACCEL %i %x\r\n", ok, buf[0]);
    simple_uart_putstring(strbuf);

    while (true)
    {
        app_sched_execute();
        power_manage();
    }
}
