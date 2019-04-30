/**
 * @file checksum.c
 * @brief This file consists of all the functions required for Checksum.
 *
 * The Code has been executed on TIVA C Series TM4C1294XL on FreeRTOS.
 *
 * @author Siddhant Jajoo
 * @date 04/20/2019
 * @see https://barrgroup.com/Embedded-Systems/How-To/CRC-Calculation-C-Code
 *
 * @copyright Copyright (c) 2019
 *
 */



#include "inc/checksum.h"


void checksum_init(void)
{
    crc  rem;
    uint8_t bit;
    int dividend;

    //Compute the remainder of each possible dividend.
    for (dividend = 0; dividend < 256; ++dividend)
    {

        //Start with the dividend followed by zeros.
        rem = dividend << (WIDTH - 8);

        //Perform modulo-2 division, a bit at a time.
        for (bit = 8; bit > 0; --bit)
        {

            //Try to divide the current data bit.
            if (rem & TBIT)
            {
                rem = (rem << 1) ^ POLY;
            }
            else
            {
                rem = (rem << 1);
            }
        }

        crcTable[dividend] = rem;
    }

}


crc checksum_calc(uint8_t const msg[], int size)
{
    uint8_t data;
    crc rem = 0;
    int byte;

    //Divide the message by the polynomial, a byte at a time.
    for (byte = 0; byte < size; ++byte)
    {
        data = msg[byte] ^ (rem >> (WIDTH - 8));
        rem = crcTable[data] ^ (rem << 8);
    }

    return (rem);

}
