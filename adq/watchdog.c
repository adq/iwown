#include "nrf_gpio.h"
#include "nrf.h"
#include "i5plus.h"

void watchdog_init() {
    NRF_WDT->RREN = 1;
}

void watchdog_reload() {
    NRF_WDT->RR[0] = WDT_RR_RR_Reload;
}
