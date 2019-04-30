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

#include "checksum.h"


void checksum_init(void)
{
    crc  rem;
    uint8_t bit;
    int div;

    /*
     * Compute the rem of each possible div.
     */
    for (div = 0; div < 256; ++div)
    {
        /*
         * Start with the div followed by zeros.
         */
        rem = div << (WID - 8);

        /*
         * Perform modulo-2 division, a bit at a time.
         */
        for (bit = 8; bit > 0; --bit)
        {
            /*
             * Try to divide the current data bit.
             */
            if (rem & TBIT)
            {
                rem = (rem << 1) ^ POLY;
            }
            else
            {
                rem = (rem << 1);
            }
        }

        /*
         * Store the result into the table.
         */
        crcTable[div] = rem;
    }

}


crc checksum_calc(uint8_t const message[], int nBytes)
{
    uint8_t data;
    crc rem = 0;
    int byte;

    /*
     * Divide the message by the POLY, a byte at a time.
     */
    for (byte = 0; byte < nBytes; ++byte)
    {
        data = message[byte] ^ (rem >> (WID - 8));
        rem = crcTable[data] ^ (rem << 8);
    }

    /*
     * The final rem is the CRC.
     */
    return (rem);

}
