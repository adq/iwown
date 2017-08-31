#include "nrf_gpio.h"
#include "nrf.h"
#include "i5plus.h"
#include "power.h"
#include "softdevice_handler.h"
#include "simple_uart.h"
#include <stdio.h>


uint32_t battery_percent;

void power_init() {
    NRF_GPIO->PIN_CNF[GPIO_USB_POWER] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) \
                                       |(GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)    \
                                       |(GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)  \
                                       |(GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) \
                                       |(GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);
}

void power_dosample() {
    uint32_t err_code;

    // Configure ADC
    NRF_ADC->INTENSET   = ADC_INTENSET_END_Msk;
    NRF_ADC->CONFIG     = (ADC_CONFIG_RES_10bit                       << ADC_CONFIG_RES_Pos)     |
                          (ADC_CONFIG_INPSEL_AnalogInputNoPrescaling  << ADC_CONFIG_INPSEL_Pos)  |
                          (ADC_CONFIG_REFSEL_VBG                      << ADC_CONFIG_REFSEL_Pos)  |
                          (ADC_CONFIG_PSEL_AnalogInput7               << ADC_CONFIG_PSEL_Pos)    |
                          (ADC_CONFIG_EXTREFSEL_None                  << ADC_CONFIG_EXTREFSEL_Pos);
    NRF_ADC->EVENTS_END = 0;
    NRF_ADC->ENABLE     = ADC_ENABLE_ENABLE_Enabled;

    // Enable ADC interrupt
    err_code = sd_nvic_ClearPendingIRQ(ADC_IRQn);
    APP_ERROR_CHECK(err_code);

    err_code = sd_nvic_SetPriority(ADC_IRQn, NRF_APP_PRIORITY_LOW);
    APP_ERROR_CHECK(err_code);

    err_code = sd_nvic_EnableIRQ(ADC_IRQn);
    APP_ERROR_CHECK(err_code);

    NRF_ADC->EVENTS_END  = 0;    // Stop any running conversions.
    NRF_ADC->TASKS_START = 1;
}

uint8_t power_readusbstatus() {
    return ((NRF_GPIO->IN & (1<<GPIO_USB_POWER)) ? 1 : 0);
}

void ADC_IRQHandler(void)
{
    if (NRF_ADC->EVENTS_END != 0)
    {
        uint32_t     adc_result;
        uint16_t     batt_lvl_in_milli_volts;
        uint8_t     percentage_batt_lvl;
        // uint32_t    err_code;

        NRF_ADC->EVENTS_END     = 0;
        adc_result              = NRF_ADC->RESULT;
        NRF_ADC->TASKS_STOP     = 1;

        batt_lvl_in_milli_volts = (((adc_result * 1200) / 1024) * 12000) / 2000;
        if (batt_lvl_in_milli_volts >= 4000) {
            battery_percent = 100;
        } else if (batt_lvl_in_milli_volts >= 3900) {
            battery_percent = 100 - ((4000 - batt_lvl_in_milli_volts) / 5);
        } else if (batt_lvl_in_milli_volts >= 3800) {
            battery_percent = 80 - ((3900 - batt_lvl_in_milli_volts) / 5);
        } else if (batt_lvl_in_milli_volts >= 3700) {
            battery_percent = 60 - ((3800 - batt_lvl_in_milli_volts) / 5);
        } else if (batt_lvl_in_milli_volts >= 3550) {
            battery_percent = 40 - ((2 * (3700 - batt_lvl_in_milli_volts)) / 15);
        } else if (batt_lvl_in_milli_volts >= 3370) {
            battery_percent = 20 - ((3550 - batt_lvl_in_milli_volts) / 10);
        } else {
            // FIXME
            battery_percent = batt_lvl_in_milli_volts > 3300;
        }

        char buf[100];
        sprintf(buf, "%i %i %i\r\n", batt_lvl_in_milli_volts, (unsigned int) adc_result, percentage_batt_lvl);
        simple_uart_putstring((uint8_t*) buf);
        // percentage_batt_lvl     = battery_level_in_percent(batt_lvl_in_milli_volts);

        /*
        err_code = ble_bas_battery_level_update(&m_bas, percentage_batt_lvl);
        if (
            (err_code != NRF_SUCCESS)
            &&
            (err_code != NRF_ERROR_INVALID_STATE)
            &&
            (err_code != BLE_ERROR_NO_TX_BUFFERS)
            &&
            (err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING)
        )
        {
            APP_ERROR_HANDLER(err_code);
        }
        */
    }
}
