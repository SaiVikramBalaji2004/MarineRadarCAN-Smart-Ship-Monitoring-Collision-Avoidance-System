
#include "ecu3.h"

int main(void)
{
    CAN_Init();           // acceptance filter 0x101..0x107 + Rx ISR      
    Alarm_Init();         // LEDs, buzzer and Timer1 red-LED blink        
    LCD_Init();           // 20x4 LCD in 4-bit mode                       

    // Project title is shown for the first 10 seconds on every boot,including after a board restart (main() re-runs on reset).
               
    LCD_ShowTitle();
    DelayMs(10000);
    LCD_Clear();

    while (1)
    {
        if (collision_danger == COLLISION_DANGER)
        {
            Alarm_Set(1);           // red blink + buzzer ON              
            LCD_ShowDanger();
        }
        else
        {
            Alarm_Set(0);           // green solid, buzzer OFF            
            LCD_ShowClearPath();
        }
        DelayMs(150);
    }
}
