#include "nrf_gpio.h"
#include "i5plus.h"
#include "twi_master.h"
#include "fontrom.h"
#include "spi_master.h"
#include "app_error.h"
#include "app_util_platform.h"


static spi_master_config_t spi_fontrom = {
    SPI_Freq: SPI_FREQUENCY_FREQUENCY_M1,
    SPI_Pin_SCK: GPIO_FONTROM_SCLK,
    SPI_Pin_MISO: GPIO_FONTROM_MISO,
    SPI_Pin_MOSI: GPIO_FONTROM_MOSI,
    SPI_Pin_SS: GPIO_FONTROM_CS,
    SPI_PriorityIRQ: APP_IRQ_PRIORITY_LOW,
    SPI_CONFIG_ORDER: SPI_CONFIG_ORDER_LsbFirst,
    SPI_CONFIG_CPOL: SPI_CONFIG_CPOL_ActiveHigh,
    SPI_CONFIG_CPHA: SPI_CONFIG_CPHA_Leading,
    SPI_DisableAllIRQ: 0
};

static void fontrom_callback(spi_master_evt_t spi_master_evt) {
    // FIXME: do something!
}

void fontrom_init() {
    uint32_t err_code = spi_master_open(SPI_MASTER_0, &spi_fontrom);
    APP_ERROR_CHECK(err_code);

    spi_master_evt_handler_reg(SPI_MASTER_0, fontrom_callback);
}
