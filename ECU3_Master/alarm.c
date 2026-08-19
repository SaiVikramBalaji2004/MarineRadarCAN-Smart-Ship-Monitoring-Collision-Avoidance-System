#include "ecu3.h"

volatile unsigned char alarm_active = 0;

void Alarm_Blink_ISR(void) __irq;

void Alarm_Init(void)
{
    IODIR1 |= (GREEN_LED | RED_LED | BUZZER);
    IOCLR1  = (GREEN_LED | RED_LED | BUZZER);

    /* Timer1 : 1 MHz count, match at 500000 -> 500 ms period */
    T1PR  = 15 - 1;
    T1MR0 = 500000;
    T1MCR = 0x03;                 // interrupt + reset on MR0             
    T1IR  = 0xFF;                 // clear any pending flag               

    VICIntSelect &= ~(1UL << VIC_TIMER1);     // as IRQ                   
    VICVectCntl1  = 0x20 | VIC_TIMER1;        // slot 1 -> Timer1         
    VICVectAddr1  = (unsigned long)Alarm_Blink_ISR;
    VICIntEnable |= (1UL << VIC_TIMER1);
}

void Alarm_Set(unsigned char active)
{
    alarm_active = active;

    if (active)
    {
        IOCLR1 = GREEN_LED;       // green LED OFF                        
        IOSET1 = BUZZER;          // buzzer SOLID ON                      
        IOSET1 = RED_LED;         // red LED starts ON, ISR blinks it     
    }
    else
    {
        IOCLR1 = RED_LED;         // red LED OFF                          
        IOCLR1 = BUZZER;          // buzzer OFF                           
        IOSET1 = GREEN_LED;       // green LED SOLID ON                   
    }
}

void Alarm_Blink_ISR(void) __irq
{
    if (alarm_active)             // toggle the red LED at 500 ms          
    {
        if (IOPIN1 & RED_LED)
            IOCLR1 = RED_LED;
        else
            IOSET1 = RED_LED;
    }
    T1IR = 0x01;                  // clear MR0 interrupt flag              
    VICVectAddr = 0x00;           // end of interrupt                      
}
