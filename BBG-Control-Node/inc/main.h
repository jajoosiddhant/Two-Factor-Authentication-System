/**
 * @file main.h
 * @author Siddhant Jajoo and Satya Mehta 
 * @brief Header file for main.c
 * @version 0.1
 * @date 2019-03-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

#define OK (0)
#define FAIL (1)
#define I2C_BUS ("/dev/i2c-2")
/*Uncomment to use I2C kernel driver and comment the previous line*/
//#define I2C_BUS ("/dev/myi2c_char")

//Timer initialization macros
#define TIMER_RETRY (1)
#define TIMER_HB (3)

#define TEMP_UNIT (0) //Set 0 for degree celsius, 1 for kelvin, 2 for fahrenheit.

// //Heartbeat values corresponding to different threads 
// #define TEMP_HB (1)
// #define LIGHT_HB (2)
// #define LOGGER_HB (3)
// #define SOCKET_HB (4)
// #define CLEAR_HB (5)

//Log Levels
#define INFO (0x01)
#define WARNING (0x02)
#define ERROR (0x04)
#define DEBUG (0x08)
#define ERROR_DEBUG (0x0C)
#define INFO_DEBUG (0x09)
#define INFO_ERROR_DEBUG (0x0E)

//Prioirty levels
#define P0		(0)
#define P1		(1)
#define P2		(2)

//Packet formats
#define PREAMBLE 0xAB
#define POSTAMBLE 0xBA
#define ACK (0x01)

//ID's # to be sent in the packet for identification
#define RANDOM_TEST (0)
#define FINGER_PRINT_ID (1)
#define OTP_SEND (2)
#define OTP_RECEIVE (3)
#define ACCESS_STATUS (4)
#define LOG_MSG_ID (5)
#define ACK_PACKET (6)
#define GUI_ID (7)

#define FINGRPRINT_MATCHED (1)
#define FINGERPRINT_UNMATCHED (0)
#define DONT_CARE (0)
#define BUZZER (1)
#define REG_NEW_FP (2)
//Mutex declarations
pthread_mutex_t mutex_a;
pthread_mutex_t mutex_b;
pthread_mutex_t mutex_error;

//Global Variables
char *filename;
volatile uint8_t retry_flag;
volatile uint8_t light_timerflag;
uint8_t g_ll;
uint8_t main_exit;
volatile uint8_t socket_flag;
int gpio_fd[2];
int r_no[4];
// Error number
typedef uint32_t err_t;


//Macros for different events.
#define ERROR_RCV_ID (3)
#define MSG_RCV_ID (4)
#define MSG_RCV_REMOTE_ID (5)

//Macros for Nodes
#define CONTROL_NODE (0)
#define REMOTE_NODE (1)


//Error structure
struct error_struct
{
	struct timespec data_time;
	err_t error_value;
	char error_str[50];
};

//Message structure
struct msg_struct
{
	struct timespec data_time;
	char msg_str[50];
};

//Main sensor structure
typedef struct
{
	uint8_t id;
	union msg_data {
		struct msg_struct msg_data;
		struct error_struct error_data;
	}msg;
}logger_struct;

// struct __attribute__((__packed__)) packet_struct
// {
// 	uint8_t preamble;
// 	uint8_t id;
// 	uint16_t size;
// 	uint8_t* payload;
// 	uint8_t ack;
// 	uint8_t crc;
// 	uint8_t postamble;
// };

struct __attribute__((__packed__))packet_struct
{
	uint8_t preamble; 
	uint8_t id;
	uint16_t size;
	uint8_t payload[50];
	uint8_t ack;
	uint16_t crc;
	uint8_t postamble;
};

//Function Declarations
err_t create_threads(char *filename);
void *logger_thread(void *filename);
logger_struct read_error(char *error_str);
logger_struct read_msg(char *msg_str, uint8_t);
void error_log(char *error_str, uint8_t loglevel, uint8_t prio);
void msg_log(char *msg_str, uint8_t loglevel, uint8_t prio, uint8_t node);
struct packet_struct make_packet(uint8_t id, uint16_t size, uint8_t *payload, uint8_t ack);
void hb_send(uint8_t hb_value);
uint8_t hb_receive(void);
uint8_t* otp_generate(void);
void hb_handle(uint8_t hb_rcv);
void retry(struct packet_struct);
uint8_t send_otp(uint8_t);
err_t mutex_init(void);
err_t mutex_destroy(void);
err_t thread_destroy(void);
err_t destroy_all(void);


#endif
