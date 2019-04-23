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
#include "inc/myuart.h"




//******************************************************************************************/
//                     Size for Command, Acknowledge and Data Packets
//******************************************************************************************/
#define COMMAND_SIZE            (12)
#define RESPONSE_SIZE           (12)
//#define DATA_SIZE               ()


//******************************************************************************************/
//                     Packets for Command, Acknowledge and Data
//******************************************************************************************/
#define START_CODE1             (0x55)
#define START_CODE2             (0xAA)
#define DEVICEID_LSBYTE         (0x01)
#define DEVICEID_MSBYTE         (0x00)
#define PARAMETER_BYTES         (0x00)
#define CMD_CODE_MSBYTE         (0x00)
#define CHECKSUM_BYTES          (0x00)


//******************************************************************************************/
//                              COMMANDS FOR GT521F32
//******************************************************************************************/
#define FP_OPEN_CMD             (0x01)
#define FP_CLOSE_CMD            (0x02)
#define FP_CMOSLED_CMD          (0x12)
#define FP_GETENROLLCOUNT_CMD   (0x20)
#define FP_CHECKENROLLED_CMD    (0x21)
#define FP_ENROLLSTART_CMD      (0x22)
#define FP_ENROLL1_CMD          (0x23)
#define FP_ENROLL2_CMD          (0x24)
#define FP_ENROLL3_CMD          (0x25)
#define FP_ISPRESSFINGER_CMD    (0x26)
#define FP_VERIFY_CMD           (0x50)
#define FP_IDENTIFY_CMD         (0x51)
#define FP_CAPTUREFINGER_CMD    (0x60)
#define FP_GETIMAGE_CMD         (0x62)
#define FP_GETRAWIMAGE_CMD      (0x63)
#define ACK                     (0x30)
#define NACK                    (0x31)


//******************************************************************************************/
//                              PARAMETERS FOR COMMAND PACKET.
//******************************************************************************************/
#define FP_LEDON                (1)
#define FP_LEDOFF               (0)

//******************************************************************************************/
//                     GPIO PORT and PINS assigned for ICPCK with GT-521F32
//******************************************************************************************/
#define FP_IRQ_PORT            (GPIO_PORTL_BASE)        /**< Port for Fingerprint sensor interrupt*/
#define FP_IRQ_PIN             (GPIO_PIN_3)             /**< Pin for Fingerprint sensor interrupt*/



//******************************************************************************************/
//                              FUNCTION DECLARATIONS
//******************************************************************************************/

/**
 * @brief This function initializes and enables the required GPIO pins interrupts for fingerprint sensor GT-521F32.
 * @return void.
 */
void fp_interrupt_enable(void);


void fp_cmdsend(uart_t uart, uint8_t cmd, uint8_t param);


void fp_responsercv(uart_t uart);


uint16_t fp_checksum_calc(uint8_t packet[]);


/**
 * @brief Callback function for Fingerprint Interrupt Handler.
 * @return void.
 */
void fp_irqhandler(void);

#endif /* INC_FINGERPRINT_H_ */
