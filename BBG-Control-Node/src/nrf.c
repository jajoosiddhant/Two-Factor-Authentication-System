/**
 * @file nrf.c
 * @author Satya Mehta
 * @brief 
 * @version 0.1
 * @date 2019-04-17
 * SPI_Test https://raw.githubusercontent.com/torvalds/linux/master/tools/spi/spidev_test.c
 * @copyright Copyright (c) 2019
 * 
 */
#include "nrf.h"

/**
 * @brief SPI driver initialization
 * SPI device opened and configurations are done like mode, bits per word, speed
 * @return uint8_t 
 */
uint8_t spi_init()
{
    int ret = 0;

    spi_fd = open(device, O_RDWR);
    if (spi_fd < 0)
    {
        error_log("ERROR: Open failed in spi_init()", ERROR_DEBUG, P2);
        perror("ERROR: Open failed in spi_init()");
    }
    ret = ioctl(spi_fd, SPI_IOC_WR_MODE32, &mode);
    if (ret == -1)
    {
        error_log("ERROR: Ioctl failed setting mode", ERROR_DEBUG, P2);
        perror("ERROR: Ioctl failed setting mode");
    }
    ret = ioctl(spi_fd, SPI_IOC_RD_MODE32, &mode);
    if (ret == -1)
    {
        error_log("ERROR: Ioctl failed in reading bits", ERROR_DEBUG, P2);
        perror("ERROR: Ioctl failed in reading bits");
    }
    ret = ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
    {
        error_log("ERROR: Ioctl failed in WR bits", ERROR_DEBUG, P2);
        perror("ERROR: Ioctl failed in WR bit");
    }
    ret = ioctl(spi_fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1)
    {
        error_log("ERROR: Ioctl failed in RD bits", ERROR_DEBUG, P2);
        perror("ERROR: Ioctl failed in RD bits");
    }
    ret = ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
    {
        perror("ERROR: Ioctl failed in set speed");
    }
    ret = ioctl(spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1)
    {
        error_log("ERROR: Ioctl failed in reading speed", ERROR_DEBUG, P2);
        perror("ERROR: Ioctl failed in reading speed");
    }
    return 0;
}

/**
 * @brief This function is used to set the Pipe addresses in NRF
 * 
 * @return uint8_t (Returns 0 on success and 1 on failure)
 */
uint8_t nrf_set_addr()
{
    int ret;
    struct rx_pipe_p0 a;
	a.rx_address[0] = 0x04;
	a.rx_address[1] = 0x00;
	a.rx_address[2] = 0x00;
	a.rx_address[3] = 0x00;
	a.rx_address[4] = 0x00;
	spi_write_mul_bytes(RX_PIPE, (a.rx_address), 5);
	ret = spi_read_mul_bytes(RX_PIPE, 5);
    for(int i = 4; i >=0; i--)
    {
        //printf("%d",rx_buff[i]);
    }

    struct tx_pipe b;
	b.tx_address[0] = 0x06;
	b.tx_address[1] = 0x00;
	b.tx_address[2] = 0x00;
	b.tx_address[3] = 0x00;
	b.tx_address[4] = 0x00;
	spi_write_mul_bytes(TX_PIPE, (b.tx_address), 5);
	ret = spi_read_mul_bytes(TX_PIPE, 5);

    for(int i = 4; i >=0; i--)
    {
        // printf("%d",rx_buff[i]);
    }
//    printf("\n");

}

/**
 * @brief This function is used to initialize NRF module
 * 
 * @return uint8_t (Returns 0 on success and 1 on failure)
 */

uint8_t nrf_init()
{
    spi_write(0x00, 0x00);
    spi_write(0x01, 0x01);
    spi_write(0x07, 0x70);
    spi_write(0x02, 0x01);
    spi_write(0x03, 0x03);
    spi_write(0x04, 0x08);
    spi_write(0x06, 0x26);
    nrf_set_addr();
    spi_write(0x11, 0x04);
    spi_write(0x06, 0x06);    //Power and Speed
    spi_write(0x05, 0x22); //set frequency channel;
    spi_write(0x00, 0x0A);
    command_nrf(TX_PAYLOAD);
    gpio_ctrl(GPIO51, GPIO51_V, 1); //set CE low
}

/**
 * @brief Wrtes the packed into the SPI Module
 * 
 * @return uint32_t (Returns 0 on success and 1 on failure)
 */
uint32_t tx_packet()
{
    spi_write(00, 0x0A);
}

/**
 * @brief Receives data from the the SPI buffers. 
 * Sends RX_Payload command to the command_nrf function to retrieve the data payload
 * Clears the rx interrupt
 * @return uint32_t 
 */
uint32_t rx_packet()
{
    int ret;
    uint8_t received_pipe;
    spi_read(STATUS);
    received_pipe = (read_status & RX_P_NO_MASK) >> 1;
    if(read_status & RX_DR_MASK)
    {
        ret =  command_nrf(RX_PAYLOAD);
        if(ret == 2)
        {
            for(int  i = 0; i < 10; i++)
            {
                //printf("Bytes rcvd %d\n", rx_buff[i]);
            }   
        }
        spi_write(STATUS, 0x40);
        spi_read(STATUS);
        gpio_ctrl(GPIO51, GPIO51_V, 0); //set CE low
        command_nrf(RX_FLUSH);
    }
}

/**
 * @brief GPIO 60 of beaglebone set from high to low.
 * GPIO60 connected to the chip select pin of the NRF module
 */
void gpio60_h2l(void)
{
    gpio_ctrl(GPIO60, GPIO60_V, 1);
    gpio_ctrl(GPIO60, GPIO60_V, 0);
}

/**
 * @brief GPIO60 of beaglebone set from lo to high
 * GPIO60 connected to the chip select pin of the NRF module
 */
void gpio60_l2h(void)
{
       gpio_ctrl(GPIO60, GPIO60_V, 1);
}

/**
 * @brief Can be used to write different commands to NRF MOdule throigh the spi interface
 * 
 * @param comm - Specifies command. Macros are created for all the available commands
 * @return uint8_t Returns 0 on success and 1 on failure
 */
uint8_t command_nrf(uint8_t comm)
{
    int ret;
    uint8_t tx_buffer = comm;
    gpio60_h2l();
	ret = write(spi_fd, &tx_buffer, 1);
    if(ret < 1)
    {
        perror("write failed\n\n");
    }
    if(comm == RX_PAYLOAD)
    {
	    ret = read(spi_fd, rx_buff, 4);
        if(ret < 1)
        {
            perror("Read failed\n");
            return 0;
        }
        return 2;
        gpio60_l2h();
    }
    if(comm == TX_PAYLOAD)
    {
        uint8_t data[4] = {0x10, 0x04, 0x50, 0x60};
        ret = write(spi_fd, &data, 4);
    }
	
}
/**
 * @brief This function configures the NF module in RX or TX mode
 * 
 * @param mode -Specifies mode as defined in the mode_t enum
 * @return uint8_t  Returns 0 on success and 1 on failure
 */
uint8_t nrf_configure_mode(mode_t mode)
{
    if (mode == mode_rx)
    {

        command_nrf(RX_FLUSH);
        spi_write(0x00, 0x0B); //PRIM_RX =1, and PWR_UP = 1
        spi_write(0x01, 0x00); //disable ACK
        spi_write(0x11, 0x01); //set RX_PW_P0
        spi_read(0x11);
        nrf_set_addr();
        gpio_ctrl(GPIO51, GPIO51_V, 1); //set CE high
        usleep(130);
    }
    if(mode == mode_tx)
    {
        spi_write(0x00, 0x0A); //Tx mode
        
    }
}


uint8_t config_tx(void)
{
    spi_write(0x01, 0x01); //Enable acknowledge
    
}

/**
 * @brief Read data from the NRF registers through the SPI interface
 * 
 * @param reg - Specifies the register address
 * @return uint8_t returns 0 on success and 1 on failure
 */
uint8_t spi_read(uint8_t reg)
{
    int ret;
    uint8_t tx_buffer = reg;
    uint8_t rx_buffer = 0;
	gpio60_h2l();
	ret = write(spi_fd, &tx_buffer, 1);
    if(ret < 1)
    {
        perror("write failed\n\n");
    }
	ret = read(spi_fd, &read_status, 1);
    if(ret < 1)
    {
        perror("Read failed\n");
    }
	gpio60_l2h();
}

/**
 * @brief This function writes a byte into the registers of NRF module through SPI interface
 * 
 * @param reg - Specify the regiser address to which data is to be written
 * @param data - Specify the data which is written
 * @return uint8_t - Returns 0 on success and 1 on failure.
 */
uint8_t spi_write(uint8_t reg, uint8_t data)
{
    int ret;
    uint8_t tx_buffer = reg|WRITE_MASK;
    uint8_t rx_buffer;
	gpio60_h2l();
	ret = write(spi_fd, &tx_buffer, 1);
    if(ret < 1)
    {
        perror("write failed\n\n");
    }
	tx_buffer = data;
	ret = write(spi_fd, &tx_buffer, 1);
    if(ret < 1)
    {
        perror("write failed\n\n");
    }
	gpio60_l2h();
}
/**
 * @brief This function writes multiple bytes into the registers of NRF module through SPI interface
 * 
 * @param reg - Specify the regiser address to which data is to be written
 * @param *data - Specify the data which is written
 * @param bytes- No of bytes to write
 * @return uint8_t - Returns 0 on success and 1 on failure.
 */

uint8_t spi_write_mul_bytes(uint8_t reg, uint8_t *data, uint8_t bytes)
{
    int ret;
    uint8_t tx_buffer = reg|WRITE_MASK;
	gpio60_h2l();
	ret = write(spi_fd, &tx_buffer, 1);
    if(ret < 1)
    {
        perror("write failed\n\n");
    }
	//tx_buffer = data;
	ret = write(spi_fd, data, bytes);
    if(ret < 1)
    {
        perror("write failed\n\n");
    }
	gpio60_l2h();
}

/**
 * @brief Reads multiple bytes from the NRF registers
 * 
 * @param reg - Specify the register address to read
 * @param bytes = Specifies the number of bytes to read
 * @return uint8_t Returns 0 on success and 1 on failure
 */
uint8_t spi_read_mul_bytes(uint8_t reg, uint8_t bytes)
{
    int ret;
    uint8_t tx_buffer = reg;
    gpio60_h2l();
	ret = write(spi_fd, &tx_buffer, 1);
    if(ret < 1)
    {
        perror("write failed\n\n");
    }
	ret = read(spi_fd, &rx_buff, bytes);
    if(ret < 1)
    {
        perror("Read failed\n");
    }
	gpio60_l2h();
}