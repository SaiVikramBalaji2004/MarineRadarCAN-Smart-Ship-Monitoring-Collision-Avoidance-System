#include "ecu3.h"

#define AF_RAM_BASE ((volatile unsigned long *)0xE0038000UL)
#define AF_STD_ENTRY(controller, id) \
    (((unsigned long)(controller) << 13) | (unsigned long)(id))
#define AF_CAN1_CONTROLLER  1

/* ---------------- shared telemetry variables --------------------------- */
volatile unsigned char nav_angle      = 0;
volatile unsigned char nav_distance   = 0;
volatile unsigned char collision_danger = COLLISION_SAFE;
volatile unsigned char eng_temp       = 0;
volatile unsigned char fire_alarm     = 0;
volatile unsigned char gas_alarm      = 0;
volatile unsigned char water_alarm    = 0;

void CAN1_Rx_ISR(void) __irq;

void CAN_Init(void)
{
    PINSEL1 |= 0x00040000;        // P0.25 = RD1 (CAN1)                   
    C1MOD = 0x01;                 // enter reset mode                     
    C1BTR = CAN_BTR_500K;         // bit timing for 500 kbps              

    /* ---------------- configure the acceptance filter ------------------ */
    AFMR = 0x01;                  // AccOff : allow filter configuration  

    AF_RAM_BASE[0] =  AF_STD_ENTRY(AF_CAN1_CONTROLLER, CAN_ID_OBJ_ANGLE)
                   | (AF_STD_ENTRY(AF_CAN1_CONTROLLER, CAN_ID_OBJ_DIST) << 16);
    AF_RAM_BASE[1] =  AF_STD_ENTRY(AF_CAN1_CONTROLLER, CAN_ID_COLLISION)
                   | (AF_STD_ENTRY(AF_CAN1_CONTROLLER, CAN_ID_ENGINE_TEMP) << 16);
    AF_RAM_BASE[2] =  AF_STD_ENTRY(AF_CAN1_CONTROLLER, CAN_ID_FIRE_STATUS)
                   | (AF_STD_ENTRY(AF_CAN1_CONTROLLER, CAN_ID_GAS_STATUS) << 16);
    AF_RAM_BASE[3] =  AF_STD_ENTRY(AF_CAN1_CONTROLLER, CAN_ID_WATER_LEVEL)
                   | (0xFFFFUL << 16);      // padding entry : disabled   

    SFF_sa     = 0x00;            // individual std table at RAM base     
    SFF_GRP_sa = 0x10;            // no standard groups                   
    EFF_sa     = 0x10;            // no extended individual entries       
    EFF_GRP_sa  = 0x10;           // no extended groups                   
    ENDofTable = 0x10;            // 4 words -> end of table              
    AFMR = 0x00;                  // enable acceptance filter              
    C1MOD = 0x00;                 // request to leave reset mode           
    while (C1MOD & 0x01);         // wait until controller leaves reset    

    /* ---------------- enable the CAN1 receive interrupt ---------------- */
    C1IER = 0x01;                 // RIE : receive interrupt enable        

    VICIntSelect &= ~(1UL << VIC_CAN1_RX);    // as IRQ                   
    VICVectCntl0  = 0x20 | VIC_CAN1_RX;       // slot 0 -> CAN1 Rx        
    VICVectAddr0  = (unsigned long)CAN1_Rx_ISR;
    VICIntEnable |= (1UL << VIC_CAN1_RX);
}

void CAN1_Rx_ISR(void) __irq
{
    unsigned long rid;
    unsigned char d0;
    if (C1GSR & 0x01)             // RBS : a message is waiting           \
    {
        rid = C1RID;
        d0  = (unsigned char)(C1RDA & 0xFF);   // first data byte         

        switch (rid)
        {
        case CAN_ID_OBJ_ANGLE:   nav_angle      = d0; break;
        case CAN_ID_OBJ_DIST:    nav_distance   = d0; break;
        case CAN_ID_COLLISION:   collision_danger = d0; break;
        case CAN_ID_ENGINE_TEMP: eng_temp       = d0; break;
        case CAN_ID_FIRE_STATUS: fire_alarm     = d0; break;
        case CAN_ID_GAS_STATUS:  gas_alarm      = d0; break;
        case CAN_ID_WATER_LEVEL: water_alarm    = d0; break;
        default: break;
        }

        C1CMR = 0x04;             // RRB : release receive buffer          
    }
    VICVectAddr = 0x00;           // end of interrupt                      
}
