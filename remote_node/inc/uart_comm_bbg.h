/**
 * @file uart_comm_bbg.h
 * @brief Header file for uart_comm_bbg.c.
 *
 * The file consists of all the function declarations, macros and datatype declarations for UART Communication.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/20/2019
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef INC_UART_COMM_BBG_H_
#define INC_UART_COMM_BBG_H_

#include "inc/main.h"
#include "inc/myuart.h"



#define BAUDRATE_BBG                    (115200)


//******************************************************************************************/
//                           FUNCTION DECLARATIONS
//******************************************************************************************/

/**
 * @brief This function configures the UART to communicate with beaglebone.
 * @return void.
 */
void configureUART_bbg(void);


/**
 * @brief Interrupt Handler for UART Communication with BBG.
 * @return void.
 */
void uart_commhandler(void);


#endif /* INC_UART_COMM_BBG_H_ */
