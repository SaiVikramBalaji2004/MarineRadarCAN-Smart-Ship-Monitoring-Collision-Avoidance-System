/*
 * ---------------------------------------------------------------------------
 *  ECU-2 Timer helpers (blocking delays) using Timer0
 * ---------------------------------------------------------------------------
 */
#include "ecu2.h"

void DelayMs(unsigned int ms)
{
    T0PR  = 15000 - 1;            /* 1 ms per tick @ 15 MHz PCLK          */
    T0TCR = 0x03;                 /* reset counter                        */
    T0TCR = 0x01;                 /* enable counter                       */
    while (T0TC < ms);            /* wait for the required time           */
    T0TCR = 0x03;                 /* reset counter                        */
    T0TCR = 0x00;                 /* disable counter                      */
}
