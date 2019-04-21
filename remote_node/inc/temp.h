/*
 * temp.h
 *
 *  Created on: Apr 8, 2019
 *      Author: Siddhant Jajoo
 */

#ifndef INC_TEMP_H_
#define INC_TEMP_H_

#include "inc/main.h"


#define CONFIG_TEMP     (0x00)
#define SLAVE_ADDR      (0x48)          //Slave Address of I2C sensor.
#define THRESHOLD       (25)


//Function Declarations
uint8_t temptask_init(void);
static void temp(void *pvParameters);
void temp_send(void);
float temp_read(void);

#endif /* INC_TEMP_H_ */
