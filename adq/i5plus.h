#ifndef I5_PLUS_H
#define I5_PLUS_H

#define I2C_BITRATE 50000

#define GPIO_LED 22

#define GPIO_OLED_SDA 23
#define GPIO_OLED_SCL 24
#define GPIO_OLED_RES 25
#define GPIO_OLED_POWER 30

#define GPIO_MOTOR 29

#define GPIO_FONTROM_MISO 20
#define GPIO_FONTROM_CS 19
#define GPIO_FONTROM_SCLK 18
#define GPIO_FONTROM_MOSI 17

#define GPIO_ACCEL_SDA 13
#define GPIO_ACCEL_SCL 12

#define GPIO_UART_RX 11
#define GPIO_UART_TX 10

#define GPIO_EEPROM_SDA 9
#define GPIO_EEPROM_SCL 8

#define GPIO_USB_POWER 5

#define GPIO_TOUCH1 4
#define GPIO_TOUCH0 3

#define I2C_EEPROM 0xA0
#define I2C_ACCEL 0x32
#define I2C_OLED 0x78

#define APP_GPIOTE_MAX_USERS            1                                           /**< Maximum number of users of the GPIOTE handler. */

#define APP_TIMER_PRESCALER             0                                           /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_MAX_TIMERS            9                                           /**< Maximum number of simultaneously created timers. */
#define APP_TIMER_OP_QUEUE_SIZE         9                                           /**< Size of timer operation queues. */

#define SCHED_MAX_EVENT_DATA_SIZE       sizeof(app_timer_event_t)                   /**< Maximum size of scheduler events. Note that scheduler BLE stack events do not contain any data, as the events are being pulled from the stack in the event handler. */
#define SCHED_QUEUE_SIZE                29                                          /**< Maximum number of events in the scheduler queue. */

#define MANUFACTURER_NAME "ADQ"
#define DEVICE_NAME "ADQ WATCH"

#define PNP_ID_VENDOR_ID_SOURCE          0x02                                           /**< Vendor ID Source. */
#define PNP_ID_VENDOR_ID                 0x1915                                         /**< Vendor ID. */
#define PNP_ID_PRODUCT_ID                0xEEEE                                         /**< Product ID. */
#define PNP_ID_PRODUCT_VERSION           0x0001                                         /**< Product Version. */

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(500, UNIT_1_25_MS)            /**< Minimum acceptable connection interval (0.5 seconds). */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(1000, UNIT_1_25_MS)           /**< Maximum acceptable connection interval (1 second). */
#define SLAVE_LATENCY                   0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds). */
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)  /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER) /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                           /**< Number of attempts before giving up the connection parameter negotiation. */

#define APP_ADV_INTERVAL                64                                          /**< The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */
#define APP_ADV_TIMEOUT_IN_SECONDS      180                                         /**< The advertising timeout (in units of seconds). */

#define DFU_REV_MAJOR                        0x00                                                    /** DFU Major revision number to be exposed. */
#define DFU_REV_MINOR                        0x05                                                    /** DFU Minor revision number to be exposed. */
#define DFU_REVISION                         ((DFU_REV_MAJOR << 8) | DFU_REV_MINOR)                  /** DFU Revision number to be exposed. Combined of major and minor versions. */


#endif
