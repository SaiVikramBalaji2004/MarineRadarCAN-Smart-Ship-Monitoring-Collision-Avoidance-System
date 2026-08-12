/*
 * ---------------------------------------------------------------------------
 *  Marine RadarCAN - ECU-2 : Ship Safety Monitoring Node
 *  NXP LPC2129 (ARM7TDMI-S) | CAN 2.0B @ 500 kbps
 *
 *  Role      : Engine bay and environmental hazard telemetry
 *  CAN bus   : CAN2  (RD2 = P0.23, TD2 = P0.24) via MCP2551
 *  Sensors   : LM35  - P0.28 (AD0.1, 10-bit ADC) -> engine temperature
 *              Flame - P0.10 (digital input)        (1 = fire)
 *              Gas   - P0.11 (digital input)        (1 = gas leak)
 *              Water - P0.12 (digital input)        (1 = bilge flooding)
 *  Transmits : 0x104 Engine Temp | 0x105 Fire | 0x106 Gas | 0x107 Water
 * ---------------------------------------------------------------------------
 */
#ifndef __ECU2_H__
#define __ECU2_H__

#include <LPC21xx.h>

/* ------------------------ CAN frame structure -------------------------- */
typedef struct {
    unsigned long id;        /* 11-bit CAN identifier               */
    unsigned char dlc;       /* data length code (0..8)             */
    unsigned char data[8];   /* payload                             */
} CAN_Message_t;

/* ---------------------- CAN identifier dictionary ---------------------- */
#define CAN_ID_ENGINE_TEMP  0x104
#define CAN_ID_FIRE_STATUS  0x105
#define CAN_ID_GAS_STATUS   0x106
#define CAN_ID_WATER_LEVEL  0x107

#define SENSOR_SAFE         0x00
#define SENSOR_ALARM        0xFF

/* -------------------- LM35 temperature sensor -------------------------- */
/* P0.28 = AIN1 (AD0.1) : PINSEL1[25:24] = 01                            */
#define LM35_PINSEL        (1UL << 24)
#define LM35_ADCR_VALUE    0x00200602UL   /* PDN=1, CLKDIV=6, ch=AD0.1  */
#define MAX_ENGINE_TEMP    150

/* -------------------- digital hazard sensor inputs --------------------- */
#define FLAME_PIN  (1UL << 10)   /* P0.10 */
#define GAS_PIN    (1UL << 11)   /* P0.11 */
#define WATER_PIN  (1UL << 12)   /* P0.12 */

/* ------------------- CAN bit timing (500 kbps) ------------------------- */
#define CAN_BTR_500K     0x00480000UL

/* ----------------------- ECU-2 prototypes ------------------------------ */
void CAN_Init(void);
void CAN_SendMessage(CAN_Message_t *msg);

void Sensors_Init(void);
unsigned char LM35_ReadTemp(void);       /* returns degrees C (0..150)  */
unsigned char Sensor_State(unsigned long pin);   /* 0x00 or 0xFF       */

void DelayMs(unsigned int ms);

#endif /* __ECU2_H__ */
