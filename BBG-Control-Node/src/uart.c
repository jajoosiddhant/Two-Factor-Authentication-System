/**
 * @file uart.c
 * @author Satya Mehta (you@domain.com)
 * @brief Thhis file contains UART configurations and read-write functions
 * @version 0.1
 * @date 2019-04-27
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "uart.h"

/**
 * @brief Initializes UART device on Beaglebone
 * UART configurations are done using termios libraries.
 * Baudrate set to 115200, Reading in blocking state
 * @return uint8_t 
 */
uint8_t uart_init(void)
{
    int ret;
    struct termios term;
    uart_fd = open(UART_PORT, O_RDWR|O_NOCTTY|O_NDELAY);
    if(uart_fd == -1)
    {
        perror("UART Open failed\n");
        return 1;
    }
    else
    fcntl(uart_fd, F_SETFL, 0);
    tcgetattr(uart_fd, &term);
    ret =  cfsetispeed(&term, B115200);
    printf("RET from set cfispeed %d\n", ret);
    if(ret == -1)
    {
        perror("Input baud failed");
        return 1;
    }
    ret = cfsetospeed(&term, B115200);
    printf("RET from set cfispeed %d\n", ret);
    if(ret == -1)
    {
        perror("Output baud failed");
        return 1;
    } 

    term.c_cflag |= (CLOCAL);
   // term.c_cflag &= ~CSIZE;
    term.c_cflag |= CS8;
   // term.c_cflag &= ~PARENB;

    term.c_lflag &= ~(ICANON | ECHO | ECHONL | ISIG | IEXTEN);
    term.c_iflag &= ~(ISTRIP | IXON | INLCR | PARMRK | ICRNL | IGNBRK);
    term.c_oflag &=  ~(OPOST);

    tcsetattr(uart_fd, TCSAFLUSH, &term);

    return 0;
}

/**
 * @brief This function sends data to the UART terminal using write system call
 * 
 * @param data - Buffer address from which data is to be written
 * @param bytes - No of bytes to be written from the buffer
 * @return uint8_t Returns 0 if success and 1 if no data received. 
 */
uint8_t send_uart(int *data, uint8_t bytes)
{
    int ret;
    ret = write(uart_fd, data, bytes);
    if(ret < 0)
    {
        perror("Write failed");
        return 1;
    }
    return 0;
}

/**
 * @brief This function sends a single byte to the UART terminal
 * 
 * @param data - Byte to be written
 * @return uint8_t Returns 0 if success and 1 if no data written. 
 */
uint8_t send_byte_uart(uint8_t data)
{
    int ret;
    ret = write(uart_fd, &data, 1);
    if(ret != 1)
    {
        perror("Write one byte through UART failed");
    }
}

/**
 * @brief This function is used to read a single byte from the UART
 * 
 * @return uint8_t Returns 0 if success and 1 if no data received. 
 */
uint8_t rcv_byte_uart()
{
    int ret;
    uint8_t data;
    ret = read(uart_fd, &data, 1);
    if(ret != 1)
    {
        perror("Write one byte through UART failed");
    }
    return data;
}

/**
 * @brief This function reads data from the to the UART terminal
 * 
 * @param data - Buffer address from which data received from the UART is stored.
 * @return uint8_t Returns 0 if success and 1 if no data written. 
 */

uint8_t rcv_uart(int *data, uint8_t bytes)
{
    int ret;
    ret = read(uart_fd, data, bytes);
    if(ret < 0)
    {
        perror("Write failed");
        return 1;
    }
    return 0;

}

/**
 * @brief Used to populate the structure with the received payload.
 * 
 * @param obj - Structure object in which payload is populated
 * @param bytes -No of bytes or the size of payload which is read from the UART buffers
 * @return struct packet_struct 
 */
struct packet_struct rcv_bytes()
{
    struct packet_struct obj;
    obj.preamble = rcv_byte_uart();
    obj.id = rcv_byte_uart();
	obj.size = rcv_byte_uart();
	obj.size = obj.size | (rcv_byte_uart() << 8);
    for(int i = 0; i < obj.size; i++)
    {
        obj.payload[i] = rcv_byte_uart();
    }
    obj.ack = rcv_byte_uart();
    obj.crc = rcv_byte_uart();
    obj.crc = obj.crc | (rcv_byte_uart() << 8);
    obj.postamble = rcv_byte_uart();
    return obj;
}
uint8_t send_bytes(struct packet_struct obj)
{
    pthread_mutex_lock(&mutex_a);
    send_byte_uart(obj.preamble);
    send_byte_uart(obj.id);
    send_byte_uart(obj.size & 0x00FF);
    send_byte_uart((obj.size & 0xFF00)>>8);
    for(int i = 0 ; i < obj.size; i++)
    {
        send_byte_uart(obj.payload[i]);   
    }
    send_byte_uart(obj.ack);
    send_byte_uart(obj.crc & 0x00FF);
    send_byte_uart((obj.crc & 0xFF00)>>8);
    send_byte_uart(obj.postamble);
    pthread_mutex_unlock(&mutex_a);
}

uint8_t send_ack()
{
    uint8_t pay[2];
    struct packet_struct ack_struct;
    pay[0] = DONT_CARE;
	ack_struct = make_packet(ACK_PACKET, 1, pay, 0);
	send_bytes(ack_struct);
}

void print_packet(struct packet_struct obj)
{
    printf("Preamble %x\n", obj.preamble);
    printf("ID %d\n", obj.id);
    printf("Size %d\n", obj.size);
    for(int i=0; i < obj.size; i++)
    printf("Payload data %d\n", obj.payload[i]);
    printf("Ack %d\n", obj.ack);
    printf("CRC %x\n", obj.crc);
    printf("Postamble %x\n", obj.postamble);

}