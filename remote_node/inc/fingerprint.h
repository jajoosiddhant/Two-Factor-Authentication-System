/**
 * @file fingerprint.h
 * @brief Header file for fingerprint.c.
 *
 * The file consists of all the function declarations, macros and typedefs related to Fingerprint functionality.
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/19/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef INC_FINGERPRINT_H_
#define INC_FINGERPRINT_H_

#include "inc/main.h"




//******************************************************************************************/
//                              FUNCTION DECLARATIONS
//******************************************************************************************/

/**
 * @brief This function configures the UART1 pins for fingerprint sensor GT-521F32.
 * @return void
 */
void configureUART_fp(void);



/**
 * @brief This function initializes and enables the required GPIO pins interrupts for fingerprint sensor GT-521F32.
 * @return void.
 */
void fp_interrupt_enable(void);


#endif /* INC_FINGERPRINT_H_ */
