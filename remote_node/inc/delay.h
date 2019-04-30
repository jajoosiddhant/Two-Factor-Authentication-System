/**
 * @file delay.h
 * @brief Header file for delay functions.
 *
 * The file consists of all the function declarations, macros and typedefs related to delay functions.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo.
 * @date 04/26/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_

#include "inc/main.h"


/**
 * @brief This function provides the specified amount of delay.
 * @param ms Time in milliseconds.
 * @return void.
 */
inline void delay_ms(uint32_t ms)
{
    SysCtlDelay((g_ui32SysClock/1000)*ms);
}


/**
 * @brief This function provides the specified amount of delay.
 * @param us Time in microseconds.
 * @return void.
 */
inline void delay_us(uint32_t us)
{
    SysCtlDelay((g_ui32SysClock/1000000)*us);
}


#endif /* INC_DELAY_H_ */
