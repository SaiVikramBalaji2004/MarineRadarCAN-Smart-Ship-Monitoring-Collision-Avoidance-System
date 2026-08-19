
#include "ecu1.h"

void CAN_Init(void)
{
    PINSEL1 |= 0x00040000;

    C1MOD = 0x01;                 // enter reset mode                     
    C1BTR = CAN_BTR_500K;         // bit timing for 500 kbps              
    C1MOD = 0x00;                 // request to leave reset mode          
    while (C1MOD & 0x01);         // wait until controller leaves reset   
}

void CAN_SendMessage(CAN_Message_t *msg)
{
    while (!(C1GSR & 0x04)) ;     // wait for a free transmit buffer      
    C1CMR  = 0x20;                // select Tx buffer 1                   
    C1TFI1 = ((unsigned long)(msg->dlc)) << 16;   // standard data frame  
    C1TID1 = msg->id;             // 11-bit identifier                    

    C1TDA1 = (unsigned long)msg->data[0]
           | ((unsigned long)msg->data[1] << 8)
           | ((unsigned long)msg->data[2] << 16)
           | ((unsigned long)msg->data[3] << 24);

    C1TDB1 = (unsigned long)msg->data[4]
           | ((unsigned long)msg->data[5] << 8)
           | ((unsigned long)msg->data[6] << 16)
           | ((unsigned long)msg->data[7] << 24);

    C1CMR  = 0x01;                /* request transmission (TR)            */
}
