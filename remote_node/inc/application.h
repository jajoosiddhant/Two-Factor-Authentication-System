/**
 * @file application.h
 * @brief Header file for application.c.
 *
 * The file consists of all the function declarations, macros and typedefs related to Application Task.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/29/2019
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef INC_APPLICATION_H_
#define INC_APPLICATION_H_

#include "inc/main.h"


/**
 * @brief This function initializes the system with the required configurations and peripherals.
 * @return void.
 */
void system_init(void);

/**
 * @brief This function creates a task.
 * @return void.
 */
uint8_t sensortask_init(void);

/**
 * @brief Callback function for sensorcheck task.
 * @param pvParameters Not in use.
 * @return void.
 */
static void sensorcheck(void *pvParameters);

#endif /* INC_APPLICATION_H_ */
