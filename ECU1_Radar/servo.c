
#include "ecu1.h"

void Servo_Init(void)
{
    PINSEL0 |= (2UL << 14);       // P0.7 = PWM2                          
    PWMPR    = 15 - 1;            // PWM timer counts at 1 MHz            
    PWMMCR   = 0x02;              // MR0 resets the timer counter         
    PWMMR0   = SERVO_PERIOD_US;   // 20000 -> 20 ms period (50 Hz)        
    PWMMR2   = 1500;              // centre position initially            
    PWMLER   = 0x05;              // latch MR0 and MR2                    
    PWMPCR   = 0x01 | (1UL << 10);// PWM mode on, PWM2 output enabled     
    PWMTCR   = 0x09;              // enable counter + PWM                 
}

void Servo_SetAngle(unsigned char angle)
{
    unsigned long pulse;

    if (angle > 180) angle = 180;

    pulse = SERVO_0DEG_US
          + ((unsigned long)angle * (SERVO_180DEG_US - SERVO_0DEG_US)) / 180UL;

    PWMMR2   = pulse;
    PWMLER  |= (1UL << 2);      
}
