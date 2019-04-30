/**
 * @file application.c
 * @brief This file consists of all the functions required for Application Thread.
 *
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/29/2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#include "inc/msg_queue.h"
#include "inc/sync_objects.h"
#include "inc/timer.h"
#include "inc/spi.h"
#include "inc/led.h"
#include "inc/nrf_driver.h"
#include "inc/buzzer.h"
#include "inc/keypad.h"
#include "inc/uart_comm_bbg.h"
#include "inc/checksum.h"
#include "inc/myuart.h"
#include "inc/fingerprint.h"
#include "inc/delay.h"
#include "inc/lcd.h"
#include "inc/packet.h"
#include "inc/application.h"


uint8_t sensortask_init(void)
{
    if(xTaskCreate(sensorcheck, (const portCHAR *)"sensorcheck", LEDTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY, NULL) != pdTRUE)
    {
        UARTprintf("Failed to create Sensor Check Task.\n");
        return(1);
    }

    return 0;
}


void system_init(void)
{
    //Global Variable Initialization.
    otp_flag = 0;
    otp_count = 0;
    otp_retries = 0;
    sensor_check = 0;
    degrade_mode = 0;
    memset(otp_arr, 0, 4);


    //Configure Timer for Retries.
    timer_config(timer_retry, PACKET_RETRY_TIME);
    timer_config(timer_otp, OTP_INPUT_TIME);

    //Initializing Checksum.
    checksum_init();

    // Initialize the UART for serial Terminal, BBG, Fingerprint Scanner.
    ConfigureUART();
    configureUART_bbg();
    uart_configure(UART_FP, CLOCK, BAUDRATE_FP, 0);

    //Initializing LCD and 4 USR LEDs.
    lcd_init();
    led_init();

    //Initializing Buzzer
    buzzer_config();
    buzzer_dutycycle(MAX_FREQ);

    //Initializing Keypad and enabling interrupts for Keypad and Fingerprint Scanner.
    keypad_config();
    keypad_interrupt_enable();
    fp_interrupt_config();

    //Initializing fingerprint.
    fp_init();


    //Create Semaphores, mutexes and queues.
    sem_create();
    mutex_create();
    queue_create();

    //TODO:/*INITIALIZATION FOR NRF*/
    //Configuring CSN and CE pins
    //nrf_gpio_init();

    //Configuring SPI pins on TIVA.
    //spi_config(SPI2);

    //nrf_config();

}


static void sensorcheck(void *pvParameters)
{

    uint8_t response_packet[RESPONSE_SIZE];
    uint8_t i;

    while(1)
    {
        delay_ms(666);

        //Get enrollment count
        fp_cmdsend(UART_FP, FP_GETENROLLCOUNT_CMD, FP_NOPARAM);


        for(i=0; i < RESPONSE_SIZE; i++)
        {
            response_packet[i] = (uint8_t)(UARTCharGetNonBlocking(UART_FP) & 0x000000FF);
        }

        if(response_packet[8] == ACK)
        {
            printf("FINGERPRINT SENSOR online: %x.\n", response_packet[8]);
            sensor_check = 0;
            otp_flag = 0;
            fp_interrupt_enable();
            LCD_write("Press Finger on Scanner");
            fp_led_status(UART_FP, FP_LEDON);
            led_off(LED4);
        }
        else
        {
            printf("Sensor Offline %x.\n", response_packet[8]);
            sensor_check = 1;
            fp_interrupt_disable();
            otp_flag = 1;
            LCD_write("Fingerprint Sensor offline: Enter PASSCODE");
            led_on(LED4);
        }
    }

}



