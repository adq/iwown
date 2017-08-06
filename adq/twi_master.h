 /* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#ifndef TWI_MASTER_H
#define TWI_MASTER_H

/*lint ++flb "Enter library region" */

#include <stdbool.h>
#include <stdint.h>

/** @file
* @brief Software controlled TWI Master driver.
*
*
* @defgroup lib_driver_twi_master Software controlled TWI Master driver
* @{
* @ingroup nrf_drivers
* @brief Software controlled TWI Master driver.
*
* Supported features:
* - Repeated start
* - No multi-master
* - Only 7-bit addressing
* - Supports clock stretching (with optional SMBus style slave timeout)
* - Tries to handle slaves stuck in the middle of transfer
*/

typedef struct
{
    uint8_t TWI_Pin_SCL;       /**< SCL pin number. */
    uint8_t TWI_Pin_SDA;       /**< SDA pin number. */
} twi_master_config_t;

#define TWI_READ_BIT                 (0x01)        //!< If this bit is set in the address field, transfer direction is from slave to master.

#define TWI_ISSUE_STOP               ((bool)true)  //!< Parameter for @ref twi_master_transfer
#define TWI_DONT_ISSUE_STOP          ((bool)false) //!< Parameter for @ref twi_master_transfer

#define TWI_DELAY() nrf_delay_us(4) /*!< Time to wait when pin states are changed. For fast-mode the delay can be zero and for standard-mode 4 us delay is sufficient. */


/**
 * @brief Function for initializing TWI bus IO pins and checks if the bus is operational.
 *
 * Both pins are configured as Standard-0, No-drive-1 (open drain).
 *
 * @return
 * @retval true TWI bus is clear for transfers.
 * @retval false TWI bus is stuck.
 */
bool twi_master_init(twi_master_config_t twi_master_config);

/**
 * @brief Function for transferring data over TWI bus.
 *
 * If TWI master detects even one NACK from the slave or timeout occurs, STOP condition is issued
 * and the function returns false.
 * Bit 0 (@ref TWI_READ_BIT) in the address parameter controls transfer direction;
 * - If 1, master reads data_length number of bytes from the slave
 * - If 0, master writes data_length number of bytes to the slave.
 *
 * @note Make sure at least data_length number of bytes is allocated in data if TWI_READ_BIT is set.
 * @note @ref TWI_ISSUE_STOP
 *
 * @param address Data transfer direction (LSB) / Slave address (7 MSBs).
 * @param data Pointer to data.
 * @param data_length Number of bytes to transfer.
 * @param issue_stop_condition If @ref TWI_ISSUE_STOP, STOP condition is issued before exiting function. If @ref TWI_DONT_ISSUE_STOP, STOP condition is not issued before exiting function. If transfer failed for any reason, STOP condition will be issued in any case.
 * @return
 * @retval true Data transfer succeeded without errors.
 * @retval false Data transfer failed.
 */
bool twi_master_transfer(twi_master_config_t twi_master_config, uint8_t address, uint8_t *data, uint8_t data_length, bool issue_stop_condition);

/**
 *@}
 **/

/*lint --flb "Leave library region" */
#endif //TWI_MASTER_H
