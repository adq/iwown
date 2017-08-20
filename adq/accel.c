#include "nrf_gpio.h"
#include "i5plus.h"
#include "twi_master.h"
#include "accel.h"


static twi_master_config_t twi_accelerometer = {TWI_Pin_SCL:GPIO_ACCEL_SCL, TWI_Pin_SDA:GPIO_ACCEL_SDA};

void accel_init() {
    twi_master_init(twi_accelerometer);
}
