#ifndef BLE_H
#define BLE_H 1

#include "ble_bas.h"

extern void ble_stack_init();
extern void advertising_start();
extern ble_bas_t                        m_bas;

#endif
