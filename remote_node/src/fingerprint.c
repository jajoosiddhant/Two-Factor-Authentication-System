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




void fp_interrupt_enable(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL))
    {
    }

    GPIOPinTypeGPIOInput(FP_IRQ_PORT, FP_IRQ_PIN);
    GPIOPadConfigSet(FP_IRQ_PORT, FP_IRQ_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPD);

    GPIOIntRegister(FP_IRQ_PORT, fp_irqhandler);

    GPIOIntTypeSet(FP_IRQ_PORT, FP_IRQ_PIN, GPIO_RISING_EDGE);

    GPIOIntEnable(FP_IRQ_PORT, FP_IRQ_PIN);
}


void fp_cmdsend(uart_t uart, uint8_t cmd, uint8_t param)
{
    uint8_t cmd_packet[COMMAND_SIZE] =
    {
     START_CODE1, START_CODE2,
     DEVICEID_LSBYTE, DEVICEID_MSBYTE,
     param, PARAMETER_BYTES, PARAMETER_BYTES, PARAMETER_BYTES,
     cmd, CMD_CODE_MSBYTE,
     CHECKSUM_BYTES, CHECKSUM_BYTES
    };

    uint8_t i;
    uint16_t fp_checksum = fp_checksum_calc(cmd_packet);

    cmd_packet[10] = fp_checksum & 0x00FF;
    cmd_packet[11] = (fp_checksum & 0xFF00)>>8 ;

    for(i = 0; i < COMMAND_SIZE; i++)
    {
        UARTCharPut(uart, cmd_packet[i]);
        uart_busy(uart);
    }
}


void fp_responsercv(uart_t uart)
{
    uint8_t response_packet[RESPONSE_SIZE];
    uint8_t i;
    uint16_t fp_checksum;

    for(i=0; i < RESPONSE_SIZE; i++)
    {
        response_packet[i] = (uint8_t)(UARTCharGet(uart) & 0x000000FF);
    }

    fp_checksum = fp_checksum_calc(response_packet);

    if (response_packet[11] == fp_checksum)
    {
        printf("CHecksum Calculated , Correct Data Received.\n");
    }
    else
    {
        printf("Checksum Failed.\n");
    }

    if(response_packet[8] == ACK)
    {
        printf("Acknowledge Received Successfully.\n");
        printf("Parameter Received: %x.\n", response_packet[4]);
    }
    else if(response_packet[8] == NACK)
    {
        printf("NACK Received.\n");
        printf("ERROR CODE Received: %x%x.\n", response_packet[4], response_packet[5]);
    }
}

uint16_t fp_checksum_calc(uint8_t packet[])
{
    uint8_t i;
    uint16_t fp_checksum = 0;
    for(i = 0; i < 10; i++)
    {
        fp_checksum += packet[i];
    }

    return fp_checksum;
}


void fp_irqhandler(void)
{
    uint32_t int_status;

    IntMasterDisable();

    // Get the interrrupt status.
    int_status = GPIOIntStatus(FP_IRQ_PORT, false);

    if (int_status & FP_IRQ_PIN)
    {
        GPIOIntClear(FP_IRQ_PORT, int_status & FP_IRQ_PIN);
        printf("Fingerprint Sensor Touched.\n");
//        SysCtlDelay(1600);
    }
    IntMasterEnable();
}

