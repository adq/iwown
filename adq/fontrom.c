#include <stdio.h>
#include "nrf_gpio.h"
#include "i5plus.h"
#include "twi_master.h"
#include "fontrom.h"
#include "spi_master.h"
#include "app_error.h"
#include "app_util_platform.h"
#include "simple_uart.h"


static const spi_master_config_t spi_fontrom = {
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
    char buf[100];

    sprintf(buf, "SPI: %i %i\r\n", spi_master_evt.evt_type, spi_master_evt.data_count);
    simple_uart_putstring((uint8_t*) buf);
}

void fontrom_init() {
    uint32_t err_code = spi_master_open(SPI_MASTER_0, &spi_fontrom);
    APP_ERROR_CHECK(err_code);

    spi_master_evt_handler_reg(SPI_MASTER_0, fontrom_callback);
}

uint32_t fontrom_read_bytes(uint32_t addr, uint8_t *rxbuf, uint16_t rxcount) {
    uint8_t txbuf[4];
    txbuf[0] = 3;
    txbuf[1] = addr >> 16;
    txbuf[2] = addr >> 8;
    txbuf[3] = addr;

    return spi_master_send_recv(SPI_MASTER_0, txbuf, 4, rxbuf, rxcount);
}
