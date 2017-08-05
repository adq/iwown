/*
 * This file is part of Espruino, a JavaScript interpreter for Microcontrollers
 *
 * Copyright (C) 2013 Gordon Williams <gw@pur3.co.uk>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * ----------------------------------------------------------------------------
 * I2C Utility functions, and software I2C
 * ----------------------------------------------------------------------------
 */

#include "i2c.h"
#include "nrf_gpio.h"
#include "app_error.h"

const int I2C_TIMEOUT = 100000;

static void dly(i2cBus *inf) {
  volatile int i;
  for (i=inf->delay;i>0;i--);
}

static void write_pin(uint32_t pin, uint32_t value) {
  nrf_gpio_pin_dir_set(pin, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_write(pin, value);
}

static uint32_t read_pin(uint32_t pin) {
  nrf_gpio_pin_dir_set(pin, NRF_GPIO_PIN_DIR_INPUT);
  return nrf_gpio_pin_read(pin);
}

static void i2c_start(i2cBus *inf) {
  if (inf->started) {
    // reset
    write_pin(inf->pinSDA, 1);
    dly(inf);
    write_pin(inf->pinSCL, 1);
    int timeout = I2C_TIMEOUT;
    while (!read_pin(inf->pinSCL) && --timeout); // clock stretch
    if (!timeout) APP_ERROR_HANDLER(0);
    dly(inf);
  }
  if (!read_pin(inf->pinSDA)) APP_ERROR_HANDLER(0);
  write_pin(inf->pinSDA, 0);
  dly(inf);
  write_pin(inf->pinSCL, 0);
  dly(inf);
  inf->started = true;
}

static void i2c_stop(i2cBus *inf) {
  write_pin(inf->pinSDA, 0);
  dly(inf);
  write_pin(inf->pinSCL, 1);
  int timeout = I2C_TIMEOUT;
  while (!read_pin(inf->pinSCL) && --timeout); // clock stretch
  if (!timeout) APP_ERROR_HANDLER(0);
  dly(inf);
  write_pin(inf->pinSDA, 1);
  dly(inf);
  if (!read_pin(inf->pinSDA)) APP_ERROR_HANDLER(0);
  dly(inf);
  inf->started = false;
}

static void i2c_wr_bit(i2cBus *inf, bool b) {
  write_pin(inf->pinSDA, b);
  dly(inf);
  write_pin(inf->pinSCL, 1);
  dly(inf);
  int timeout = I2C_TIMEOUT;
  while (!read_pin(inf->pinSCL) && --timeout); // clock stretch
  if (!timeout) APP_ERROR_HANDLER(0);
  write_pin(inf->pinSCL, 0);
  write_pin(inf->pinSDA, 1); // stop forcing SDA (needed?)
}

static bool i2c_rd_bit(i2cBus *inf) {
  write_pin(inf->pinSDA, 1); // stop forcing SDA
  dly(inf);
  write_pin(inf->pinSCL, 1); // stop forcing SDA
  int timeout = I2C_TIMEOUT;
  while (!read_pin(inf->pinSCL) && --timeout); // clock stretch
  if (!timeout) APP_ERROR_HANDLER(0);
  dly(inf);
  bool b = read_pin(inf->pinSDA);
  write_pin(inf->pinSCL, 0);
  return b;
}

// true on ack, false on nack
static bool i2c_wr(i2cBus *inf, uint8_t data) {
  int i;
  for (i=0;i<8;i++) {
    i2c_wr_bit(inf, data&128);
    data <<= 1;
  }
  return !i2c_rd_bit(inf);
}

static uint8_t i2c_rd(i2cBus *inf, bool nack) {
  int i;
  int data = 0;
  for (i=0;i<8;i++)
    data = (data<<1) | (i2c_rd_bit(inf)?1:0);
  i2c_wr_bit(inf, nack);
  return data;
}

// ----------------------------------------------------------------------------

void i2cInit(i2cBus *inf, uint32_t pinSCL, uint32_t pinSDA, uint32_t bitrate) {
  inf->pinSCL = pinSCL;
  inf->pinSDA = pinSDA;
  inf->delay = 4000000/bitrate;
  inf->started = false;

  // FIXME: setup for i2c pins!
}

void i2cWrite(i2cBus *inf, unsigned char address, int nBytes, const unsigned char *data, bool sendStop) {
  i2c_start(inf);
  i2c_wr(inf, address<<1);
  int i;
  for (i=0;i<nBytes;i++)
    i2c_wr(inf, data[i]);
  if (sendStop) i2c_stop(inf);
}

void i2cRead(i2cBus *inf, unsigned char address, int nBytes, unsigned char *data, bool sendStop) {
  i2c_start(inf);
  i2c_wr(inf, 1|(address<<1));
  int i;
  for (i=0;i<nBytes;i++)
    data[i] = i2c_rd(inf, i==nBytes-1);
  if (sendStop) i2c_stop(inf);
}
