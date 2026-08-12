/*
 * ---------------------------------------------------------------------------
 *  ECU-3 20x4 character LCD driver (HD44780), 4-bit mode
 *  Data/control lines on Port 0 : RS=P0.16, EN=P0.17, D4..D7=P0.18..P0.21
 *  RW is tied to ground, so the LCD is write-only from the MCU side.
 *  Row start addresses : row1=0x80, row2=0xC0, row3=0x94, row4=0xD4
 * ---------------------------------------------------------------------------
 */
#include "ecu3.h"

/* ---------- low level nibble / byte transfers ---------- */

static void LCD_Nibble(unsigned char nibble)
{
    IOCLR0 = LCD_DATA_MASK;       /* clear the four data lines            */
    if (nibble & 0x01) IOSET0 = LCD_D4;
    if (nibble & 0x02) IOSET0 = LCD_D5;
    if (nibble & 0x04) IOSET0 = LCD_D6;
    if (nibble & 0x08) IOSET0 = LCD_D7;

    IOSET0 = LCD_EN;              /* E high                               */
    DelayUs(2);
    IOCLR0 = LCD_EN;              /* E low  -> data latched               */
}

static void LCD_Byte(unsigned char rs, unsigned char value)
{
    if (rs)
        IOSET0 = LCD_RS;          /* RS = 1 : data register               */
    else
        IOCLR0 = LCD_RS;          /* RS = 0 : command register            */

    LCD_Nibble((unsigned char)(value >> 4));   /* high nibble first       */
    LCD_Nibble((unsigned char)(value & 0x0F)); /* then low nibble         */
    DelayUs(50);
}

/* ---------- public interface ---------- */

void LCD_Cmd(unsigned char cmd)
{
    LCD_Byte(0, cmd);
}

void LCD_Data(unsigned char ch)
{
    LCD_Byte(1, ch);
}

void LCD_Init(void)
{
    /* P0.16 .. P0.21 as GPIO (clear their PINSEL1 fields) */
    PINSEL1 &= ~(0x00000FFFUL);
    IODIR0  |= (LCD_RS | LCD_EN | LCD_DATA_MASK);

    DelayMs(20);

    /* wake-up sequence : 8-bit commands, then switch to 4-bit mode */
    LCD_Nibble(0x03);
    DelayMs(5);
    LCD_Nibble(0x03);
    DelayMs(2);
    LCD_Nibble(0x03);
    DelayMs(2);
    LCD_Nibble(0x02);             /* function set : interface = 4-bit     */
    DelayMs(2);

    LCD_Cmd(0x28);                /* 4-bit, 2 lines, 5x8 dots             */
    LCD_Cmd(0x0C);                /* display ON, cursor OFF, no blink     */
    LCD_Cmd(0x06);                /* increment address, no display shift  */
    LCD_Cmd(0x01);                /* clear display                        */
    DelayMs(2);
}

void LCD_Clear(void)
{
    LCD_Cmd(0x01);
    DelayMs(2);
}

void LCD_SetCursor(unsigned char row, unsigned char col)
{
    static const unsigned char row_addr[4] = { 0x80, 0xC0, 0x94, 0xD4 };

    if (row == 0 || row > 4) row = 1;
    if (col == 0 || col > 20) col = 1;
    LCD_Cmd((unsigned char)(row_addr[row - 1] + (col - 1)));
}

void LCD_String(const char *str)
{
    while (*str)
        LCD_Data((unsigned char)*str++);
}

void LCD_Number(unsigned int value)
{
    char buf[5];
    int  i = 0;

    if (value == 0)
    {
        LCD_Data('0');
        return;
    }
    while (value > 0 && i < 4)
    {
        buf[i++] = (char)('0' + (value % 10));
        value   /= 10;
    }
    while (i > 0)
        LCD_Data((unsigned char)buf[--i]);
}


void LCD_ShowTitle(void)
{
    LCD_SetCursor(1, 1); LCD_String("CAN-Based Smart Ship");
    LCD_SetCursor(2, 1); LCD_String("Monitoring and");
    LCD_SetCursor(3, 1); LCD_String("Collision Warning");
    LCD_SetCursor(4, 1); LCD_String("System");
}

void LCD_ShowClearPath(void)
{
    LCD_SetCursor(1, 1); LCD_String("RADAR: CLEAR");

    LCD_SetCursor(2, 1); LCD_String("OBJ DIST: ");
    if (nav_distance > 0) LCD_Number(nav_distance);
    else                  LCD_String("NONE");

    LCD_SetCursor(3, 1); LCD_String("ENG: ");
    LCD_Number(eng_temp); LCD_String("C | WTR: ");
    LCD_String(water_alarm ? "FLOOD" : "OK");

    LCD_SetCursor(4, 1); LCD_String("FIRE: ");
    LCD_String(fire_alarm ? "YES" : "NO");
    LCD_String(" | GAS: ");
    LCD_String(gas_alarm ? "YES" : "NO");
}

void LCD_ShowDanger(void)
{
    LCD_SetCursor(1, 1); LCD_String("!!! DANGER !!!");

    LCD_SetCursor(2, 1); LCD_String("OBJ AT: ");
    LCD_Number(nav_angle); LCD_String(" DEG");

    LCD_SetCursor(3, 1); LCD_String("ENG: ");
    LCD_Number(eng_temp); LCD_String("C | WTR: ");
    LCD_String(water_alarm ? "FLOOD" : "OK");

    LCD_SetCursor(4, 1); LCD_String("FIRE: ");
    LCD_String(fire_alarm ? "YES" : "NO");
    LCD_String(" | GAS: ");
    LCD_String(gas_alarm ? "YES" : "NO");
}
