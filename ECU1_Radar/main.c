#include "ecu1.h"

static CAN_Message_t msg;

static void SendRadarFrame(unsigned char angle, unsigned int distance)
{
    unsigned char obstacle;
    obstacle = (distance > 0) ? COLLISION_DANGER : COLLISION_SAFE;

    msg.id  = CAN_ID_OBJ_ANGLE;
    msg.dlc = 1;
    msg.data[0] = angle;
    CAN_SendMessage(&msg);

    msg.id      = CAN_ID_OBJ_DIST;
    msg.data[0] = (distance > 255) ? 255 : (unsigned char)distance;
    CAN_SendMessage(&msg);

    msg.id      = CAN_ID_COLLISION;
    msg.data[0] = obstacle;
    CAN_SendMessage(&msg);
}

int main(void)
{
    unsigned int  distance;
    unsigned char angle;

    CAN_Init();
    Servo_Init();
    HC_SR04_Init();

    while (1)
    {
        for (angle = 0; angle <= 180; angle += 10)
        {
            Servo_SetAngle(angle);
            DelayMs(150);                
            distance = HC_SR04_ReadDistance();
            SendRadarFrame(angle, distance);
            DelayMs(100);
        }
        for (angle = 180; angle > 0; angle -= 10)
        {
            Servo_SetAngle(angle);
            DelayMs(150);

            distance = HC_SR04_ReadDistance();
            SendRadarFrame(angle, distance);
            DelayMs(100);
        }
    }
}
