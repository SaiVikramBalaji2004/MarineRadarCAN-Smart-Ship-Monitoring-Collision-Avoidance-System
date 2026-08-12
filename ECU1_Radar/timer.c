
#include "ecu1.h"

void DelayMs(unsigned int ms)
{
    T1PR  = 15000 - 1;            /* 1 ms per tick @ 15 MHz PCLK          */
    T1TCR = 0x03;                 /* reset counter                        */
    T1TCR = 0x01;                 /* enable counter                       */
    while (T1TC < ms);            /* wait for the required time           */
    T1TCR = 0x03;                 /* reset counter                        */
    T1TCR = 0x00;                 /* disable counter                      */
}

void DelayUs(unsigned int us)
{
    T1PR  = 15 - 1;               /* 1 us per tick @ 15 MHz PCLK          */
    T1TCR = 0x03;                 /* reset counter                        */
    T1TCR = 0x01;                 /* enable counter                       */
    while (T1TC < us);            /* wait for the required time           */
    T1TCR = 0x03;                 /* reset counter                        */
    T1TCR = 0x00;                 /* disable counter                      */
}
