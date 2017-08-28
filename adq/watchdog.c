#include "nrf_gpio.h"
#include "nrf.h"
#include "i5plus.h"
#include "simple_uart.h"

void watchdog_init() {
    NRF_WDT->RREN = 1;
}

void watchdog_reload() {
    simple_uart_putstring((uint8_t*) "WDT RELOAD\r\n");

    NRF_WDT->RR[0] = WDT_RR_RR_Reload;
}
