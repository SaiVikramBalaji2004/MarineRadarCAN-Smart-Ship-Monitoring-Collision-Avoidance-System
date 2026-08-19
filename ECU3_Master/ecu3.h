
#ifndef __ECU3_H__
#define __ECU3_H__

#include <LPC21xx.h>

typedef struct {
    unsigned long id;        // 11-bit CAN identifier               
    unsigned char dlc;       // data length code (0..8)             
    unsigned char data[8];   // payload                             
} CAN_Message_t;

/* ---------------------- CAN identifier dictionary ---------------------- */
#define CAN_ID_OBJ_ANGLE    0x101
#define CAN_ID_OBJ_DIST     0x102
#define CAN_ID_COLLISION    0x103
#define CAN_ID_ENGINE_TEMP  0x104
#define CAN_ID_FIRE_STATUS  0x105
#define CAN_ID_GAS_STATUS   0x106
#define CAN_ID_WATER_LEVEL  0x107

#define COLLISION_SAFE      0x00
#define COLLISION_DANGER    0xFF

/* ------------------------ alarm indicators ----------------------------- */
#define GREEN_LED  (1UL << 16)   // P1.16 : SAFE                         
#define RED_LED    (1UL << 17)   // P1.17 : DANGER (blinks)              
#define BUZZER     (1UL << 18)   // P1.18 : active alarm                 

/* ------------------- 20x4 LCD (4-bit mode, Port 0) --------------------- */
#define LCD_RS  (1UL << 16)      // P0.16 : register select              
#define LCD_EN  (1UL << 17)      // P0.17 : enable strobe                 
#define LCD_D4  (1UL << 18)      // P0.18 : data nibble                   
#define LCD_D5  (1UL << 19)      // P0.19                                 
#define LCD_D6  (1UL << 20)      // P0.20                                 
#define LCD_D7  (1UL << 21)      // P0.21                                 
#define LCD_DATA_MASK (LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7)
                                  // RW is tied to GND (write only)       */

/* ------------------- CAN bit timing (500 kbps) ------------------------- */
#define CAN_BTR_500K     0x00480000UL

/* ----------------------- interrupt channels ---------------------------- */
#define VIC_CAN1_RX      26       // CAN1 Rx interrupt channel on LPC2129  
#define VIC_TIMER1       5        // Timer1 interrupt channel              

/* ---------------- shared telemetry (updated by CAN1 Rx ISR) ------------ */
extern volatile unsigned char nav_angle;
extern volatile unsigned char nav_distance;
extern volatile unsigned char collision_danger;
extern volatile unsigned char eng_temp;
extern volatile unsigned char fire_alarm;
extern volatile unsigned char gas_alarm;
extern volatile unsigned char water_alarm;

/* ----------------------- ECU-3 prototypes ------------------------------ */
void CAN_Init(void);
void Alarm_Init(void);
void Alarm_Set(unsigned char active);

void LCD_Init(void);
void LCD_Clear(void);
void LCD_Cmd(unsigned char cmd);
void LCD_Data(unsigned char ch);
void LCD_SetCursor(unsigned char row, unsigned char col);
void LCD_String(const char *str);
void LCD_Number(unsigned int value);
void LCD_ShowClearPath(void);
void LCD_ShowDanger(void);
void LCD_ShowTitle(void);

void DelayMs(unsigned int ms);
void DelayUs(unsigned int us);

#endif 
