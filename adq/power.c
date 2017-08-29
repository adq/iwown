#include "nrf_gpio.h"
#include "nrf.h"
#include "i5plus.h"
#include "power.h"


void power_init() {
    NRF_GPIO->PIN_CNF[GPIO_USB_POWER] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) \
                                       |(GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)    \
                                       |(GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)  \
                                       |(GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) \
                                       |(GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);
}

uint8_t power_readusbstatus() {
    return ((NRF_GPIO->IN & GPIO_USB_POWER) ? 1 : 0);
}
