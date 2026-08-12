
#include "ecu1.h"

void HC_SR04_Init(void)
{
    IODIR0  |= TRIG_PIN;          /* trigger pin as output                */
    IOCLR0   = TRIG_PIN;          /* trigger low                          */
    PINSEL1 |= ECHO_PINSEL;       /* P0.16 = CAP0.2 (capture input)       */
}

unsigned int HC_SR04_ReadDistance(void)
{
    unsigned long start, stop;
    unsigned long timeout;

    /* Send a 10 us trigger pulse on P0.8 */
    IOSET0 = TRIG_PIN;
    DelayUs(15);
    IOCLR0 = TRIG_PIN;
                                 */
    T0PR  = 15 - 1;
    T0TCR = 0x03;                 /* reset timer                          */
    T0IR  = 0xFF;                 /* clear all interrupt flags            */
    T0CCR = 0x1C0;                /* capture rising + falling, CAP0.2     */
    T0TCR = 0x01;                 /* enable timer                         */

    timeout = 0;
    while (!(T0IR & 0x40))
    {
        if (++timeout > 2000000UL)   /* no echo -> no obstacle            */
        {
            T0TCR = 0x00;
            T0CCR = 0x00;
            return 0;
        }
    }
    start = T0CR2;
    T0IR  = 0x40;                 /* clear CR2 flag                       */

    timeout = 0;
    while (!(T0IR & 0x40))
    {
        if (++timeout > 2000000UL)
        {
            T0TCR = 0x00;
            T0CCR = 0x00;
            return 0;
        }
    }
    stop = T0CR2;

    T0TCR = 0x00;                 /* stop timer                           */
    T0CCR = 0x00;                 /* disable capture                      */

    if (stop > start)
    {
        unsigned long width = stop - start;    /* echo width in us       */
        unsigned int  dist  = (unsigned int)(width / 58UL);   /* cm       */

        if ((dist < SENSOR_MIN_CM) || (dist > SENSOR_MAX_CM))
            return 0;             /* outside usable range                 */
        return dist;
    }
    return 0;
}
