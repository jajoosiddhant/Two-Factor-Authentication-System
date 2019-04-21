/**
 * @file timer.c
 * @brief This file consists of all the functions required for Timer initialization, enable and callback functions.
 *
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/13/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#include "inc/timer.h"
#include "inc/sync_objects.h"



void timer_config(uint8_t timer_x)
{
    if (timer_x == TIMER0A)
    {

        // Enable the peripherals for Timers.
        ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

        // Configure the 32-bit periodic timer.
        ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
        ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, TEMP_INTERVAL);

        // Setup the interrupts for the timer timeout.
        ROM_IntEnable(INT_TIMER0A);
        ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

        // Enable the timers.
        ROM_TimerEnable(TIMER0_BASE, TIMER_A);
    }
    else if (timer_x == TIMER1A)
    {

        // Enable the peripherals for Timers.
        ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

        // Configure the 32-bit periodic timer.
        ROM_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
        ROM_TimerLoadSet(TIMER1_BASE, TIMER_A, LED_INTERVAL);

        // Setup the interrupts for the timer timeout.
        ROM_IntEnable(INT_TIMER1A);
        ROM_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

        // Enable the timers.
        ROM_TimerEnable(TIMER1_BASE, TIMER_A);

    }
}


void enable_timer_int(void)
{
    // Enable processor interrupts.
    ROM_IntMasterEnable();
}


void temp_timer0handler(void)
{
    //Disable all interrupts
    ROM_IntMasterDisable();

    // Clear the timer interrupt.
    ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    //Release the Semaphore for the Temperature Task
    xSemaphoreGive(g_temp);

    //Enable all interrupts
    ROM_IntMasterEnable();
}


void led_timer0handler(void)
{

}
