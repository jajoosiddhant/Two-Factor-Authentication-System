/**
 * @file uart_comm_bbg.c
 * @brief This file consists of all the functions required for the UART Communication between TIVA and Beaglebone, TIVA and Fingerprint sensor GT521F32.
 *
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/19/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#include <inc/uart_comm_bbg.h>



//Change for generalised UARt configuration.
void configureUART_bbg(void)
{
    // Enable the GPIO Peripheral used by the UART.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);

    // Enable UART0
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);

    // Configure GPIO Pins for UART mode.
    ROM_GPIOPinConfigure(GPIO_PK0_U4RX);
    ROM_GPIOPinConfigure(GPIO_PK1_U4TX);
    ROM_GPIOPinTypeUART(GPIO_PORTK_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Use the internal 16MHz oscillator as the UART clock source.
    UARTClockSourceSet(UART4_BASE, UART_CLOCK_PIOSC);

    // Initialize the UART for console I/O.
//    UARTStdioConfig(UART6_BASE, BAUDRATE_BBG, CLOCK);



    UARTConfigSetExpClk(UART4_BASE, CLOCK, BAUDRATE_BBG, UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8);

//    UARTFIFOEnable(UART6_BASE);

//    UARTFIFOLevelSet(UART6_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
    UARTTxIntModeSet(UART4_BASE, UART_TXINT_MODE_EOT);

    uart_comm_flushRX(UART4);
    uart_comm_flushTX(UART4);

    UARTIntRegister(UART4_BASE, uart_commhandler);

    //Enabling UART interrupt for TRansmitter and Receiver.
    UARTIntEnable(UART4_BASE, UART_INT_RT | UART_INT_RX | UART_INT_TX);

    UARTEnable(UART4_BASE);

}





void uart_commhandler(void)
{

    uint32_t int_status;

    IntMasterDisable();

    // Get the interrrupt status.
    int_status = UARTIntStatus(UART4, false);


    if (int_status & UART_INT_TX)
    {
        printf("Character Sent.\n");

        // Clear the asserted interrupts.
        UARTIntClear(UART4, UART_INT_TX);
    }
    else if (int_status & UART_INT_RT || int_status & UART_INT_RX)
    {
        printf("Character Received due to RT.\n");


        while(UARTCharsAvail(UART4_BASE))
        {
            int32_t x = 0;
            x = UARTCharGetNonBlocking(UART4_BASE);

            printf("Displaying received Characters : %d\n", x);
        }

        // Clear the asserted interrupts.
        UARTIntClear(UART4, UART_INT_RX | UART_INT_RT);

    }

    IntMasterEnable();
}

