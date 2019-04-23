/**
 * @file main.h
 * @brief Header file for main.c.
 *
 * The file consists of all the function declarations, macros and datatype declarations.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/16/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "sensorlib/i2cm_drv.h"
#include "driverlib/i2c.h"
#include "utils/uartstdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "drivers/pinout.h"
#include "drivers/buttons.h"



#define LEDTASKSTACKSIZE        (128)
#define printf                  (UARTprintf)
#define CLOCK                   (16000000)


//******************************************************************************************/
//                                MACROS FOR DIFFERENT EVENTS
//******************************************************************************************/
#define RANDOM_TEST_ID  (0)
#define TEMP_RCV_ID     (1)
#define LED_RCV_ID      (2)


//******************************************************************************************/
//                        GLOBAL VARIABLE TO HOLD THE SYSTEM CLOCK SPEED
//******************************************************************************************/
uint32_t g_ui32SysClock;


//******************************************************************************************/
//                              STRUCTURES CONTAINING GLOBAL DATA
//******************************************************************************************/
struct temp
{
    TickType_t current_time;
    float temp;
    char str[50];
    int num;
};


typedef struct
{
    uint8_t id;
    union
    {
        struct temp temp_data;
    }data;
}event;



//******************************************************************************************/
//                              FUNCTION DECLARATIONS
//******************************************************************************************/

/**
 * @brief Configures the UART and its pins.This must be called before UARTprintf().
 * @return void
 */
void ConfigureUART(void);

/**
 * @brief This function enables all the required GPIO peripherals.
 * @return void.
 */
void gpio_enable(void);


#endif /* INC_MAIN_H_ */
