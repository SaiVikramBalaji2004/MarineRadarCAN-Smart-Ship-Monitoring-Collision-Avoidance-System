
#include "ecu2.h"

void Sensors_Init(void)
{
    PINSEL1 |= LM35_PINSEL;                    // P0.28 = AIN1 (AD0.1)  
    ADCR     = LM35_ADCR_VALUE;                // select AD0.1 channel   
    IODIR0  &= ~(FLAME_PIN | GAS_PIN | WATER_PIN);   // all as inputs   
}

unsigned char LM35_ReadTemp(void)
{
    unsigned long adc;
    unsigned long mv;

    ADCR |= 0x01000000UL;          // start A/D conversion               
    while (!(ADDR & 0x80000000UL));// wait until conversion is complete  
    adc = (ADDR >> 6) & 0x3FF;     // extract the 10-bit result          
    /* LM35 : 10 mV per deg C, ADC reference 3.3 V                       */
    mv  = (adc * 3300UL) / 1023UL;   // voltage in millivolts            
    mv /= 10UL;                      // convert to degrees C             

    if (mv > MAX_ENGINE_TEMP) mv = MAX_ENGINE_TEMP;
    return (unsigned char)mv;
}

unsigned char Sensor_State(unsigned long pin)
{
    return (IOPIN0 & pin) ? SENSOR_ALARM : SENSOR_SAFE;
}
