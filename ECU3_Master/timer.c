
#include "ecu3.h"

void DelayMs(unsigned int ms)
{
    T0PR  = 15000 - 1;            // 1 ms per tick @ 15 MHz PCLK          
    T0TCR = 0x03;                 // reset counter                        
    T0TCR = 0x01;                 // enable counter                       
    while (T0TC < ms);            // wait for the required time           
    T0TCR = 0x03;                 // reset counter                        
    T0TCR = 0x00;                 // disable counter                      
}

void DelayUs(unsigned int us)
{
    T0PR  = 15 - 1;               // 1 us per tick @ 15 MHz PCLK          
    T0TCR = 0x03;                 // reset counter                        
    T0TCR = 0x01;                 // enable counter                       
    while (T0TC < us);            // wait for the required time           
    T0TCR = 0x03;                 // reset counter                        
    T0TCR = 0x00;                 // disable counter                      
}
