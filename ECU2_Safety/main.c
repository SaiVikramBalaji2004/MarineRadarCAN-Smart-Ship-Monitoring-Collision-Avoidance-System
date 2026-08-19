
#include "ecu2.h"

static CAN_Message_t msg;

int main(void)
{
    CAN_Init();
    Sensors_Init();

    while (1)
    {
        msg.dlc = 1;

        msg.id        = CAN_ID_ENGINE_TEMP;
        msg.data[0]   = LM35_ReadTemp();
        CAN_SendMessage(&msg);

        msg.id        = CAN_ID_FIRE_STATUS;
        msg.data[0]   = Sensor_State(FLAME_PIN);
        CAN_SendMessage(&msg);

        msg.id        = CAN_ID_GAS_STATUS;
        msg.data[0]   = Sensor_State(GAS_PIN);
        CAN_SendMessage(&msg);

        msg.id        = CAN_ID_WATER_LEVEL;
        msg.data[0]   = Sensor_State(WATER_PIN);
        CAN_SendMessage(&msg);

        DelayMs(100);
    }
}
