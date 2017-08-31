#include "nrf_gpio.h"
#include "nrf.h"
#include "i5plus.h"

void touchpanel_init() {
    NRF_GPIO->PIN_CNF[GPIO_TOUCH0] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) \
                                    |(GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)    \
                                    |(GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)  \
                                    |(GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) \
                                    |(GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);

    NRF_GPIO->PIN_CNF[GPIO_TOUCH1] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) \
                                    |(GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)    \
                                    |(GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)  \
                                    |(GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) \
                                    |(GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);
}

uint8_t touchpanel_read() {
    return ((NRF_GPIO->IN & (1<<GPIO_TOUCH0)) ? 0 : 1) |
           ((NRF_GPIO->IN & (1<<GPIO_TOUCH1)) ? 0 : 2);
}
