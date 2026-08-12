
#ifndef __ECU1_H__
#define __ECU1_H__

#include <LPC21xx.h>

typedef struct {
    unsigned long id;        /* 11-bit CAN identifier               */
    unsigned char dlc;       /* data length code (0..8)             */
    unsigned char data[8];   /* payload                             */
} CAN_Message_t;

#define CAN_ID_OBJ_ANGLE    0x101
#define CAN_ID_OBJ_DIST     0x102
#define CAN_ID_COLLISION    0x103

#define COLLISION_SAFE      0x00
#define COLLISION_DANGER    0xFF


#define TRIG_PIN    (1UL << 8)    /* P0.8  : Trigger (GPIO output)   */
#define ECHO_PIN    (1UL << 16)   /* P0.16 : Echo (CAP0.2 capture)   */
#define ECHO_PINSEL 0x03UL        /* PINSEL1[1:0] = 11 -> CAP0.2     */
#define SENSOR_MAX_CM   400UL     /* HC-SR04 reliable max range      */
#define SENSOR_MIN_CM   2UL       /* HC-SR04 blind zone              */


#define SERVO_0DEG_US    1000UL   /* 1 ms  -> 0 deg                  */
#define SERVO_180DEG_US  2000UL   /* 2 ms  -> 180 deg                */
#define SERVO_PERIOD_US  20000UL  /* 20 ms -> 50 Hz frame rate       */

#define CAN_BTR_500K     0x00480000UL

void CAN_Init(void);
void CAN_SendMessage(CAN_Message_t *msg);

void Servo_Init(void);
void Servo_SetAngle(unsigned char angle);

void HC_SR04_Init(void);
unsigned int HC_SR04_ReadDistance(void);   /* cm, 0 = no valid echo */

void DelayMs(unsigned int ms);
void DelayUs(unsigned int us);

#endif 
