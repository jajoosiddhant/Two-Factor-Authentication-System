/**
 * @file fingerprint.c
 * @brief This file consists of all the functions required for the Fingerprint Scanner GT-521F32.
 *
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/19/2019
 *
 * @copyright Copyright (c) 2019
 *
 */


#include "inc/fingerprint.h"


void configureUART_fp(void)
{
//    // Enable the GPIO Peripheral used by the UART.
//    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
//
//    // Enable UART0
//    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
//
//    // Configure GPIO Pins for UART mode.
//    ROM_GPIOPinConfigure(GPIO_PB0_U1RX);
//    ROM_GPIOPinConfigure(GPIO_PB1_U1TX);
//    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
//
//    // Use the internal 16MHz oscillator as the UART clock source.
//    UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);
//
//    // Initialize the UART for console I/O.
//    UARTStdioConfig(0, 9600, 16000000);
//
//    MAP_UARTConfigSetExpClk(g_ui32SysClock, ui32SrcClock, ui32Baud,
//                            (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE |
//                                    UART_CONFIG_WLEN_8));
//
//    UARTConfigSetExpClk(UART1_BASE, g_ui32SysClock, 9600, ui32Config, (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE |
//            UART_CONFIG_WLEN_8));
//
//    UARTEnable(UART1_BASE);
}


void fp_interrupt_enable(void)
{
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
//
//    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL))
//    {
//    }
//
//    GPIOPinTypeGPIOInput(NRF_IRQ_PORT, NRF_IRQ_PIN);
//    GPIOPadConfigSet(NRF_IRQ_PORT, NRF_IRQ_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
//
//    GPIOIntRegister(NRF_IRQ_PORT, nrf_irqhandler);
//    GPIOIntTypeSet(NRF_IRQ_PORT, NRF_IRQ_PIN, GPIO_FALLING_EDGE);
//
//    GPIOIntEnable(NRF_IRQ_PORT, NRF_IRQ_PIN);
//
//    //GPIOIntRegisterPin(NRF_IRQ_PORT, NRF_IRQ_PIN, nrf_irqhandler);
//
}


