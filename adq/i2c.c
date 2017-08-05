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

const int I2C_TIMEOUT = 100000;

static void dly(i2cBus *inf) {
  volatile int i;
  for (i=inf->delay;i>0;i--);
}

static void i2c_start(i2cBus *inf) {
  if (inf->started) {
    // reset
    jshPinSetValue(inf->pinSDA, 1);
    dly(inf);
    jshPinSetValue(inf->pinSCL, 1);
    int timeout = I2C_TIMEOUT;
    while (!jshPinGetValue(inf->pinSCL) && --timeout); // clock stretch
    if (!timeout) err("Timeout (start)");
    dly(inf);
  }
  if (!jshPinGetValue(inf->pinSDA)) err("Arbitration (start)");
  jshPinSetValue(inf->pinSDA, 0);
  dly(inf);
  jshPinSetValue(inf->pinSCL, 0);
  dly(inf);
  inf->started = true;
}

static void i2c_stop(i2cBus *inf) {
  jshPinSetValue(inf->pinSDA, 0);
  dly(inf);
  jshPinSetValue(inf->pinSCL, 1);
  int timeout = I2C_TIMEOUT;
  while (!jshPinGetValue(inf->pinSCL) && --timeout); // clock stretch
  if (!timeout) err("Timeout (stop)");
  dly(inf);
  jshPinSetValue(inf->pinSDA, 1);
  dly(inf);
  if (!jshPinGetValue(inf->pinSDA)) err("Arbitration (stop)");
  dly(inf);
  inf->started = false;
}

static void i2c_wr_bit(i2cBus *inf, bool b) {
  jshPinSetValue(inf->pinSDA, b);
  dly(inf);
  jshPinSetValue(inf->pinSCL, 1);
  dly(inf);
  int timeout = I2C_TIMEOUT;
  while (!jshPinGetValue(inf->pinSCL) && --timeout); // clock stretch
  if (!timeout) err("Timeout (wr)");
  jshPinSetValue(inf->pinSCL, 0);
  jshPinSetValue(inf->pinSDA, 1); // stop forcing SDA (needed?)
}

static bool i2c_rd_bit(i2cBus *inf) {
  jshPinSetValue(inf->pinSDA, 1); // stop forcing SDA
  dly(inf);
  jshPinSetValue(inf->pinSCL, 1); // stop forcing SDA
  int timeout = I2C_TIMEOUT;
  while (!jshPinGetValue(inf->pinSCL) && --timeout); // clock stretch
  if (!timeout) err("Timeout (rd)");
  dly(inf);
  bool b = jshPinGetValue(inf->pinSDA);
  jshPinSetValue(inf->pinSCL, 0);
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

void jsi2cWrite(i2cBus *inf, unsigned char address, int nBytes, const unsigned char *data, bool sendStop) {
  if (inf->pinSCL==PIN_UNDEFINED || inf->pinSDA==PIN_UNDEFINED)
    return;
  i2c_start(inf);
  i2c_wr(inf, address<<1);
  int i;
  for (i=0;i<nBytes;i++)
    i2c_wr(inf, data[i]);
  if (sendStop) i2c_stop(inf);
}

void jsi2cRead(i2cBus *inf, unsigned char address, int nBytes, unsigned char *data, bool sendStop) {
  if (inf->pinSCL==PIN_UNDEFINED || inf->pinSDA==PIN_UNDEFINED)
    return;
  i2c_start(inf);
  i2c_wr(inf, 1|(address<<1));
  int i;
  for (i=0;i<nBytes;i++)
    data[i] = i2c_rd(inf, i==nBytes-1);
  if (sendStop) i2c_stop(inf);
}
