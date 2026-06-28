#ifndef LCD_H
#define LCD_H

#include <xc.h>

#define _XTAL_FREQ 20000000

#define COMMAND     0
#define DATA        1

#define RS      PORTCbits.RC1
#define RW      PORTCbits.RC0
#define ENABLE  PORTCbits.RC2
#define BF      PORTDbits.RD7

#define CLEAR_DISP             lcd_write(0x01,COMMAND)
#define DISP_ON_CURSOR_OFF     lcd_write(0x0C,COMMAND)

void init_lcd(void);
void lcd_write(unsigned char data, unsigned char type);
void lcd_putch(unsigned char ch,unsigned char line,unsigned char pos);
void lcd_putstr(char *str,unsigned char line,unsigned char pos);

#endif