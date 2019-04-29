#ifndef _UART_H
#define _UART_H

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>
#include "main.h"
#include "checksum.h"

/*Macros*/
#define UART_PORT "/dev/ttyO4"


/*Function declaration*/
uint8_t uart_init();
uint8_t send_uart(int*, uint8_t bytes);
uint8_t rcv_byte_uart(void);
uint8_t rcv_uart(int *, uint8_t bytes);
uint8_t send_byte_uart(uint8_t);
struct packet_struct rcv_bytes();
uint8_t send_ack(void);
uint8_t send_bytes(struct packet_struct obj);
void print_packet(struct packet_struct);

/*Global variables*/
int uart_fd;

#endif
