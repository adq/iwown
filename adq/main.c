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
#include "fontrom.h"
#include "motor.h"
#include "accel.h"
#include "OLED_SSD1306.h"
#include "app_timer.h"
#include "app_gpiote.h"
#include "app_scheduler.h"
#include "softdevice_handler.h"
#include "twi_master.h"
#include "simple_uart.h"
#include "bootloader_types.h"
#include "watchdog.h"
#include "touchpanel.h"
#include "power.h"


// NOTE: get into DFU bootloader: NRF_POWER->GPREGRET == BOOTLOADER_DFU_START  then soft reset
// sd_power_gpregret_set(BOOTLOADER_DFU_START);
// sd_nvic_SystemReset();
// use dfu_app_handler


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

static int circleon = 1;
static void circle_toggle()
{
    if (circleon) {
        OLED_fillCircle(100, 16, 10, WHITE);
    } else {
        OLED_fillCircle(100, 16, 10, BLACK);
    }
    OLED_updateDisplay();
    circleon = !circleon;
}

static void accel_thing()
{
    int16_t x,y,z;

    accel_readsample(&x, &y, &z);

    char buf[100];
    sprintf(buf, "%i %i %i\r\n", x, y, z);

    simple_uart_putstring((uint8_t*) buf);
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
    fontrom_init();
    accel_init();
    OLED_init();
    watchdog_init();
    power_init();
    touchpanel_init();
    advertising_start();

    OLED_clearDisplay();
    OLED_drawRect(0, 0, SSD1306_LCDWIDTH, SSD1306_LCDHEIGHT, WHITE);
    OLED_setTextSize(1);
    OLED_setTextColor(WHITE);
    OLED_setCursor(2, 2);
    OLED_writechars("Hello world!\r\nLine 2");
    OLED_fillCircle(100, 16, 10, WHITE);
    OLED_updateDisplay();

    // uint8_t buf[10];
    // buf[0] = 0x0d;
    // twi_master_transfer(accelerometer, 0x3a, buf, 1, false);
    // int ok = twi_master_transfer(accelerometer, 0x3a | TWI_READ_BIT, buf, 1, true);

    /*
    char buf[256];
    for(int i=0; i < 512; i++) {
        int v = eeprom_read_byte(i);
        char c;
        if ((v < 32) || (v > 126)) {
            c = ' ';
        }

        sprintf(buf, "%04x: %02x [%c]\r\n", i, v, c);
        simple_uart_putstring((uint8_t*) buf);
    }
    */

    /*
    uint8_t rxbuf[100];
    fontrom_read_bytes(0, rxbuf, 10);
*/

    simple_uart_putstring((uint8_t*) "RESTART\r\n");
    watchdog_reload();
    app_timer_id_t watchdog_timer;
    app_timer_create(&watchdog_timer, APP_TIMER_MODE_REPEATED, watchdog_reload);
    app_timer_start(watchdog_timer, APP_TIMER_TICKS(500, APP_TIMER_PRESCALER), NULL);

    led_on();

    // app_timer_id_t circle_timer;
    // app_timer_create(&circle_timer, APP_TIMER_MODE_REPEATED, circle_toggle);
    // app_timer_start(circle_timer, APP_TIMER_TICKS(500, APP_TIMER_PRESCALER), NULL);

    app_timer_id_t accel_timer;
    app_timer_create(&accel_timer, APP_TIMER_MODE_REPEATED, accel_thing);
    app_timer_start(accel_timer, APP_TIMER_TICKS(500, APP_TIMER_PRESCALER), NULL);

    simple_uart_putstring((uint8_t*) "RESTART\r\n");

    char buf[100];
    sprintf(buf, "STATUS %i\r\n", touchpanel_read());
    simple_uart_putstring((uint8_t*) buf);

    while (true)
    {
        app_sched_execute();
        power_manage();
    }
}
