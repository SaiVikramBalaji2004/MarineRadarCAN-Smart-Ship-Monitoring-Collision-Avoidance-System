#include "ecu2.h"

void CAN_Init(void)
{
    PINSEL1 |= 0x00004000 | 0x00010000;      // P0.23 = RD2, P0.24 = TD2 
    C2MOD = 0x01;                 // enter reset mode                     
    C2BTR = CAN_BTR_500K;         // bit timing for 500 kbps              
    C2MOD = 0x00;                 // request to leave reset mode          
    while (C2MOD & 0x01);         // wait until controller leaves reset   
}

void CAN_SendMessage(CAN_Message_t *msg)
{
    while (!(C2GSR & 0x04)) ;     // wait for a free transmit buffer      
    C2CMR  = 0x20;                // select Tx buffer 1                   
    C2TFI1 = ((unsigned long)(msg->dlc)) << 16;   // standard data frame  
    C2TID1 = msg->id;             // 11-bit identifier                    

    C2TDA1 = (unsigned long)msg->data[0]
           | ((unsigned long)msg->data[1] << 8)
           | ((unsigned long)msg->data[2] << 16)
           | ((unsigned long)msg->data[3] << 24);

    C2TDB1 = (unsigned long)msg->data[4]
           | ((unsigned long)msg->data[5] << 8)
           | ((unsigned long)msg->data[6] << 16)
           | ((unsigned long)msg->data[7] << 24);

    C2CMR  = 0x01;                // request transmission (TR)            
}
