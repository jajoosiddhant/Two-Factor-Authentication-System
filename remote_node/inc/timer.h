/**
 * @file timer.h
 * @brief Header file for timer.c.
 *
 * The file consists of all the function declarations, macros and typedefs related to TIMER functionality.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/12/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "inc/main.h"

//******************************************************************************************/
//                                  TIMER MACROS
//******************************************************************************************/
#define TIMER0A         (1)             /**< Used as a parameter for timer_config() function*/
#define TIMER1A         (2)             /**< Used as a parameter for timer_config() function*/
#define TEMP_INTERVAL   (120000000)     /**< This macro corresponds to 1 second*/
#define LED_INTERVAL    (12000000)      /**< This macro corresponds to 100 milliseconds*/


//******************************************************************************************/
//                              FUNCTION DECLARATIONS
//******************************************************************************************/

/**
 * @brief This function creates, initializes and enables TIMER0A and TIMER1A.
 * @param timer_x These are Macros defined to create and initialize a particular timer.
 * The Macros are TIMER0A and TIMER1A.
 * @return void.
 */
void timer_config(uint8_t timer_x);


/**
 * @brief This function enbale the Timer Interrupts.
 * @return void.
 */
void enable_timer_int(void);


/**
 * @brief Callback Function for Temperature Timer(TIMER0A).
 * @return void.
 */
void temp_timer0handler(void);


/**
 * @brief Callback Function for LED Timer(TIMER0A).
 * @return void.
 */
void led_timer0handler(void);


#endif /* INC_TIMER_H_ */
