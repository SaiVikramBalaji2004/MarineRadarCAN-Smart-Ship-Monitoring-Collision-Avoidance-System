#ifndef __ECU2_H__
#define __ECU2_H__

#include <LPC21xx.h>

/* ------------------------ CAN frame structure -------------------------- */
typedef struct {
    unsigned long id;        // 11-bit CAN identifier               
    unsigned char dlc;       // data length code (0..8)             
    unsigned char data[8];   // payload                             
} CAN_Message_t;

/* ---------------------- CAN identifier dictionary ---------------------- */
#define CAN_ID_ENGINE_TEMP  0x104
#define CAN_ID_FIRE_STATUS  0x105
#define CAN_ID_GAS_STATUS   0x106
#define CAN_ID_WATER_LEVEL  0x107

#define SENSOR_SAFE         0x00
#define SENSOR_ALARM        0xFF

/* -------------------- LM35 temperature sensor -------------------------- */
// P0.28 = AIN1 (AD0.1) : PINSEL1[25:24] = 01                            
#define LM35_PINSEL        (1UL << 24)
#define LM35_ADCR_VALUE    0x00200602UL   // PDN=1, CLKDIV=6, ch=AD0.1  
#define MAX_ENGINE_TEMP    150

/* -------------------- digital hazard sensor inputs --------------------- */
#define FLAME_PIN  (1UL << 10)   // P0.10 
#define GAS_PIN    (1UL << 11)   // P0.11 
#define WATER_PIN  (1UL << 12)   // P0.12 

/* ------------------- CAN bit timing (500 kbps) ------------------------- */
#define CAN_BTR_500K     0x00480000UL

/* ----------------------- ECU-2 prototypes ------------------------------ */
void CAN_Init(void);
void CAN_SendMessage(CAN_Message_t *msg);

void Sensors_Init(void);
unsigned char LM35_ReadTemp(void);       // returns degrees C (0..150) 
unsigned char Sensor_State(unsigned long pin);   // 0x00 or 0xFF       

void DelayMs(unsigned int ms);

#endif 
