#include "nrf_gpio.h"
#include "i5plus.h"
#include "twi_master.h"
#include "oled.h"

static twi_master_config_t twi_oled = {TWI_Pin_SCL:GPIO_OLED_SCL, TWI_Pin_SDA:GPIO_OLED_SDA};

void oled_init() {
    twi_master_init(twi_oled);
}
