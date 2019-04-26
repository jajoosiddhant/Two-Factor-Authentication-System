/**
 * @file keypad.c
 * @brief This file consists of all the functions required for the Keypad.
 *
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/19/2019
 *
 * @copyright Copyright (c) 2019
 *
 */


#include "inc/keypad.h"





char keypad_digits[ROWS][COLUMNS] = {{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};

void keypad_config(void)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);

    // Check if the peripheral access is enabled.
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOM) || !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOQ))
    {
    }


    //Configuring all Rows as Output Type
    GPIOPinTypeGPIOOutput(ROW1_PORT, ROW1_PIN);
    GPIOPinTypeGPIOOutput(ROW2_PORT, ROW2_PIN);
    GPIOPinTypeGPIOOutput(ROW3_PORT, ROW3_PIN);
    GPIOPinTypeGPIOOutput(ROW4_PORT, ROW4_PIN);

    //Configuring all the output rows as Low level.
    GPIOPinWrite(ROW1_PORT, ROW1_PIN, 0);
    GPIOPinWrite(ROW2_PORT, ROW2_PIN, 0);
    GPIOPinWrite(ROW3_PORT, ROW3_PIN, 0);
    GPIOPinWrite(ROW4_PORT, ROW4_PIN, 0);

    //Configuring all Columns as Input Type
    GPIOPinTypeGPIOInput(COLUMN1_PORT, COLUMN1_PIN);      //Column 1 from right
    GPIOPinTypeGPIOInput(COLUMN2_PORT, COLUMN2_PIN);      //Column 2 from right
    GPIOPinTypeGPIOInput(COLUMN3_PORT, COLUMN3_PIN);      //Column 3 from right
    GPIOPinTypeGPIOInput(COLUMN4_PORT, COLUMN4_PIN);      //Column 4 from right

    GPIOPadConfigSet(COLUMN1_PORT, COLUMN1_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(COLUMN2_PORT, COLUMN2_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(COLUMN3_PORT, COLUMN3_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(COLUMN4_PORT, COLUMN4_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

    GPIOIntRegister(COMMON_COLUMN_PORT, column_handler);

    GPIOIntTypeSet(COLUMN1_PORT, COLUMN1_PIN, GPIO_FALLING_EDGE);
    GPIOIntTypeSet(COLUMN2_PORT, COLUMN2_PIN, GPIO_FALLING_EDGE);
    GPIOIntTypeSet(COLUMN3_PORT, COLUMN3_PIN, GPIO_FALLING_EDGE);
    GPIOIntTypeSet(COLUMN4_PORT, COLUMN4_PIN, GPIO_FALLING_EDGE);

    GPIOIntEnable(COLUMN1_PORT, COLUMN1_PIN);
    GPIOIntEnable(COLUMN2_PORT, COLUMN2_PIN);
    GPIOIntEnable(COLUMN3_PORT, COLUMN3_PIN);
    GPIOIntEnable(COLUMN4_PORT, COLUMN4_PIN);

}


void column_handler(void)
{
    GPIOIntUnregister(COMMON_COLUMN_PORT);

    uint32_t int_status = 0;

    //Get Pin Interrupt Status.
    int_status = GPIOIntStatus(COMMON_COLUMN_PORT, false);

//    printf("Interrupt Status: %d.\n", int_status);

    if(int_status & COLUMN4_PIN)
    {
        keypad_button_detect(COLUMN4_PORT, COLUMN4_PIN, keypad_digits, COLUMN4);
    }
    else if(int_status == 4)
    {
        keypad_button_detect(COLUMN3_PORT, COLUMN3_PIN, keypad_digits, COLUMN3);
    }
    else if(int_status == 2)
    {
        keypad_button_detect(COLUMN2_PORT, COLUMN2_PIN, keypad_digits, COLUMN2);
    }
    else if(int_status == 1)
    {
        keypad_button_detect(COLUMN1_PORT, COLUMN1_PIN, keypad_digits, COLUMN1);
    }

    //Delay to prevent debouncing
    SysCtlDelay((g_ui32SysClock * 500)/1000);


    //Clear Interrupt.
    //TODO: Change only the particular triggered interrupt.
    GPIOIntClear(COMMON_COLUMN_PORT, int_status);


    GPIOIntRegister(COMMON_COLUMN_PORT, column_handler);

}



void keypad_button_detect(uint32_t column_port, uint32_t column_pin, char keypad_digits[ROWS][COLUMNS],uint8_t column)
{
    GPIOPinWrite(ROW1_PORT, ROW1_PIN, ROW1_PIN);
    if(GPIOPinRead(column_port, column_pin) & column_pin)
    {
        printf("Button Pressed: %c\n", keypad_digits[0][column]);
        GPIOPinWrite(ROW1_PORT, ROW1_PIN, 0);
        while(!(GPIOPinRead(column_port, column_pin) & column_pin));
    }
    else
    {
        GPIOPinWrite(ROW1_PORT, ROW1_PIN, 0);
        GPIOPinWrite(ROW2_PORT, ROW2_PIN, ROW2_PIN);
        if(GPIOPinRead(column_port, column_pin) & column_pin)
        {
            printf("Button Pressed: %c\n", keypad_digits[1][column]);
            GPIOPinWrite(ROW2_PORT, ROW2_PIN, 0);
            while(!(GPIOPinRead(column_port, column_pin) & column_pin));
        }
        else
        {
            GPIOPinWrite(ROW2_PORT, ROW2_PIN, 0);
            GPIOPinWrite(ROW3_PORT, ROW3_PIN, ROW3_PIN);
            if(GPIOPinRead(column_port, column_pin) & column_pin)
            {
                printf("Button Pressed: %c\n", keypad_digits[2][column]);
                GPIOPinWrite(ROW3_PORT, ROW3_PIN, 0);
                while(!(GPIOPinRead(column_port, column_pin) & column_pin));
            }
            else
            {
                GPIOPinWrite(ROW3_PORT, ROW3_PIN, 0);
                GPIOPinWrite(ROW4_PORT, ROW4_PIN, ROW4_PIN);
                if(GPIOPinRead(column_port, column_pin) & column_pin)
                {
                    printf("Button Pressed: %c\n", keypad_digits[3][column]);
                    GPIOPinWrite(ROW4_PORT, ROW4_PIN, 0);
                    while(!(GPIOPinRead(column_port, column_pin) & column_pin));
                }
            }
        }
    }
}
