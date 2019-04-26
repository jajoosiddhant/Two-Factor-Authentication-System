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


uint16_t fp_responsercv(uart_t uart)
{
    uint8_t response_packet[RESPONSE_SIZE];
    uint8_t i;
//    uint16_t fp_checksum;
    uint16_t temp;

    for(i=0; i < RESPONSE_SIZE; i++)
    {
        response_packet[i] = (uint8_t)(UARTCharGet(uart) & 0x000000FF);
    }

//    fp_checksum = fp_checksum_calc(response_packet);

//    if (response_packet[11] == fp_checksum)
//    {
//        printf("Checksum Calculated , Correct Data Received.\n");
//    }
//    else
//    {
//        printf("Checksum Failed.\n");
//    }

    if(response_packet[8] == ACK)
    {
//        printf("Acknowledge Received Successfully.\n");
//        printf("Parameter Received: %x.\n", response_packet[4]);

        temp = response_packet[5] << 8;
        temp = temp | response_packet[4];

        return (temp);
    }
    else if(response_packet[8] == NACK)
    {
        temp = response_packet[5] << 8;
        temp = temp | response_packet[4];
        printf("NACK Received.\n");
        printf("ERROR CODE Received: %x\n", temp);

        return (temp);
    }
    return -1;
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


uint16_t fp_start_enroll(uart_t uart, uint16_t new_id)
{
    uint8_t id_lsb, id_msb;
    uint8_t i;
    uint16_t fp_checksum;

    id_lsb = (uint8_t)(new_id & 0x00FF);
    id_msb = (uint8_t)(new_id & 0xFF00);


    uint8_t cmd_packet[COMMAND_SIZE] =
    {
     START_CODE1, START_CODE2,
     DEVICEID_LSBYTE, DEVICEID_MSBYTE,
     id_lsb, id_msb, PARAMETER_BYTES, PARAMETER_BYTES,
     FP_ENROLLSTART_CMD, CMD_CODE_MSBYTE,
     CHECKSUM_BYTES, CHECKSUM_BYTES
    };


    fp_checksum = fp_checksum_calc(cmd_packet);

    cmd_packet[10] = fp_checksum & 0x00FF;
    cmd_packet[11] = (fp_checksum & 0xFF00)>>8 ;

    for(i = 0; i < COMMAND_SIZE; i++)
    {
        UARTCharPut(uart, cmd_packet[i]);
        uart_busy(uart);
    }

    return (fp_responsercv(uart));
}



void fp_enroll(uart_t uart, uint8_t enroll_cmd)
{
    printf("Place finger on Scanner.\n");
    //Stays here till finger is not pressed.
    while(fp_isfingerpress(uart));

    //Capture finger.
    fp_capture(uart, FP_BEST_IMAGE);

    //Enrollment Command.
    fp_cmdsend(uart, enroll_cmd, FP_NOPARAM);
    fp_responsercv(uart);

    printf("Lift finger from Scanner.\n");
    //Stay here till finger is lifted
    while(!fp_isfingerpress(uart));
}



void add_fingerprint(uart_t uart)
{
    //TODO: Take care of duplicated ID.

    uint16_t enroll_count;
    uint32_t ui32Int;
    uint32_t int_status;

    //Disable GPIO interrupt for Finger print Sensor.
    ui32Int = IntMasterDisable();


    //LCD Display
    //Get ready to Register Fingerprint

    //Get enrollment count
    enroll_count = fp_get_enrollcount(uart);

    printf("Starting Enrollment.\n");

    fp_start_enroll(uart, enroll_count);

    //Enroll Finger print 3 times.
    fp_enroll(uart, FP_ENROLL1_CMD);
    fp_enroll(uart, FP_ENROLL2_CMD);
    fp_enroll(uart, FP_ENROLL3_CMD);


    //Clear Interrupts if in case interrupts were triggered.
    //Get Pin Interrupt Status.
    int_status = GPIOIntStatus(FP_IRQ_PORT, false);
    //Clear the particular Interrupt
    GPIOIntClear(FP_IRQ_PORT, int_status & FP_IRQ_PIN);



    // If interrupts were enabled when we turned them off, turn them
    // back on again.
    if(!ui32Int)
    {
        IntMasterEnable();
    }

}


uint16_t fp_baudrate(uart_t uart, uint16_t baudrate)
{
    uint8_t br_lsb, br_msb;
    uint8_t i;
    uint16_t fp_checksum;

    br_lsb = (uint8_t)(baudrate & 0x00FF);
    br_msb = (uint8_t)(baudrate & 0xFF00);

    uint8_t cmd_packet[COMMAND_SIZE] =
    {
     START_CODE1, START_CODE2,
     DEVICEID_LSBYTE, DEVICEID_MSBYTE,
     br_lsb, br_msb, PARAMETER_BYTES, PARAMETER_BYTES,
     FP_BAUDRATE_CMD, CMD_CODE_MSBYTE,
     CHECKSUM_BYTES, CHECKSUM_BYTES
    };

    fp_checksum = fp_checksum_calc(cmd_packet);

    cmd_packet[10] = fp_checksum & 0x00FF;
    cmd_packet[11] = (fp_checksum & 0xFF00)>>8 ;

    for(i = 0; i < COMMAND_SIZE; i++)
    {
        UARTCharPut(uart, cmd_packet[i]);
        uart_busy(uart);
    }

    return (fp_responsercv(uart));
}


uint16_t fp_deleteid(uart_t uart, uint16_t id)
{
    uint8_t id_lsb, id_msb;
    uint8_t i;
    uint16_t fp_checksum;

    id_lsb = (uint8_t)(id & 0x00FF);
    id_msb = (uint8_t)(id & 0xFF00);

    uint8_t cmd_packet[COMMAND_SIZE] =
    {
     START_CODE1, START_CODE2,
     DEVICEID_LSBYTE, DEVICEID_MSBYTE,
     id_lsb, id_msb, PARAMETER_BYTES, PARAMETER_BYTES,
     FP_DELETEID_CMD, CMD_CODE_MSBYTE,
     CHECKSUM_BYTES, CHECKSUM_BYTES
    };

    fp_checksum = fp_checksum_calc(cmd_packet);

    cmd_packet[10] = fp_checksum & 0x00FF;
    cmd_packet[11] = (fp_checksum & 0xFF00)>>8 ;

    for(i = 0; i < COMMAND_SIZE; i++)
    {
        UARTCharPut(uart, cmd_packet[i]);
        uart_busy(uart);
    }

    return (fp_responsercv(uart));
}


uint16_t fp_verify(uart_t uart, uint16_t id)
{
    uint8_t id_lsb, id_msb;
    uint8_t i;
    uint16_t fp_checksum;

    id_lsb = (uint8_t)(id & 0x00FF);
    id_msb = (uint8_t)(id & 0xFF00);

    uint8_t cmd_packet[COMMAND_SIZE] =
    {
     START_CODE1, START_CODE2,
     DEVICEID_LSBYTE, DEVICEID_MSBYTE,
     id_lsb, id_msb, PARAMETER_BYTES, PARAMETER_BYTES,
     FP_VERIFY_CMD, CMD_CODE_MSBYTE,
     CHECKSUM_BYTES, CHECKSUM_BYTES
    };

    fp_checksum = fp_checksum_calc(cmd_packet);

    cmd_packet[10] = fp_checksum & 0x00FF;
    cmd_packet[11] = (fp_checksum & 0xFF00)>>8 ;

    for(i = 0; i < COMMAND_SIZE; i++)
    {
        UARTCharPut(uart, cmd_packet[i]);
        uart_busy(uart);
    }

    return (fp_responsercv(uart));
}


uint16_t fp_identify(uart_t uart)
{
    fp_cmdsend(uart, FP_IDENTIFY_CMD, FP_NOPARAM);
    return (fp_responsercv(uart));
}



void fp_irqhandler(void)
{
    uint8_t i;
    uint16_t verify_status;
    uint32_t int_status;

    IntMasterDisable();

    // Get the interrrupt status.
    int_status = GPIOIntStatus(FP_IRQ_PORT, false);

    if (int_status & FP_IRQ_PIN)
    {
        GPIOIntClear(FP_IRQ_PORT, int_status & FP_IRQ_PIN);
        printf("Fingerprint Sensor Touched.\n");

        fp_capture(UART_FP, FP_LOW_IMAGE);


        //TODO:Use identify instead OF THE FOR LOOP below.

        for(i = 0; i < fp_get_enrollcount(UART_FP); i++)
        {
            verify_status = fp_verify(UART_FP, 0);
            printf("Verify Status: %d", verify_status);


            //Send OTP over here.
        }

//        SysCtlDelay(1600);
    }

    IntMasterEnable();
}



