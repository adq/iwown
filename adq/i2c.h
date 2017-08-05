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

#ifndef I2C_H
#define I2C_H 1

#include <stdbool.h>

typedef struct {
  int pinSCL;
  int pinSDA;
  bool started;
  int delay;
} i2cBus;

#define i2cInit(bus, pinSCL, pinSDA, bitrate) \
  bus->pinSCL = pinSCL; \
  bus->pinSDA = pinSDA; \
  bus->started = false; \
  bus->delay = 4000000/bitrate; \

void i2cWrite(i2cBus *inf, unsigned char address, int nBytes, const unsigned char *data, bool sendStop);
void i2cRead(i2cBus *inf, unsigned char address, int nBytes, unsigned char *data, bool sendStop);

#endif

