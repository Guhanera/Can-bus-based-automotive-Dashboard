#include "lcd.h"

void lcd_write(unsigned char data, unsigned char type)
{
    RS = type;
    RW = 0;

    PORTD = data;
    ENABLE = 1;
    ENABLE = 0;

    //  Poll Busy Flag
    RW = 1;
    TRISD7 = 1;
    RS = 0;

    do
    {
        ENABLE = 1;
        ENABLE = 0;
    }
    while(BF);

    RW = 0;
    TRISD7 = 0;
}


void init_lcd(void)
{
    TRISD = 0;
    TRISC = TRISC & 0XF8;

    //  LCD Init sequence
    
    __delay_ms(20);
    lcd_write(0x30,COMMAND);

    __delay_ms(5);
    lcd_write(0x30,COMMAND);

    __delay_us(100);
    lcd_write(0x30,COMMAND);

    lcd_write(0x38, COMMAND);   // function set
    __delay_ms(2);

    DISP_ON_CURSOR_OFF;
    __delay_ms(2);

    CLEAR_DISP;
    __delay_ms(2);

    lcd_write(0x06, COMMAND);
    __delay_ms(2);

}

void lcd_putch(unsigned char ch,unsigned char line,unsigned char pos)
{
    unsigned char addr = 0x80 + pos;
    if(line == 1)
    {
        addr = 0xC0 + pos;
    }

    lcd_write(addr,COMMAND);
    lcd_write(ch, DATA);
}

void lcd_putstr(char *str,unsigned char line,unsigned char pos)     //  For Line 1 -> 0 & For Line 2 -> 1
{
    unsigned char addr = 0x80 + pos;
    if(line == 1)
    {
        addr = 0xC0 + pos;
    }

    lcd_write(addr, COMMAND);

    while(*str)
    {
        lcd_write(*str, DATA);
        str++;
        __delay_us(50);
    }
}