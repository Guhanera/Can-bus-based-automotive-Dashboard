#include "ecu1_sensor.h"
#include "adc.h"
#include "can.h"
#include "digital_keypad.h"
#include "msg_id.h"
#include "uart.h"
#include "lcd.h"

#define MAX_SPEED 250U

char* gear[] = {"GN","G1","G2","G3","G4","G5","G6","GR"};
extern unsigned char collision_flag;

uint16_t get_speed(void)
{    
    uint32_t sum = 0;

    //  Averaging the adc_values
    for(unsigned char i = 0; i < 16; i++)
    {
        sum += read_adc(SPEED_ADC_CHANNEL);
    }

    uint16_t adc_val = sum >> 4;      // divide by 16

    uint16_t speed = (uint16_t)((uint32_t)adc_val * MAX_SPEED / 1023U);

    
    //  Format as "SPEED : XXX" for UART
    char speed_str[12] = "SPEED : ";
    unsigned int temp = speed;
    int digits = 0;
    unsigned int n = temp;

    do
    {
        digits++;
        n /= 10;
    } while(n);

    int idx = 8 + digits - 1;

    do
    {
        speed_str[idx--] = (temp % 10) + '0';
        temp /= 10;
    } while(temp);

    speed_str[8 + digits] = '\0';

    //  Send Speed in UART
    static unsigned int delay = 0;

    if(delay++ >= 10)
    {
        puts(speed_str);
        puts("\r\n");
        delay = 0;
    }

    return speed;
}

unsigned char get_gear_pos()
{
    static unsigned char idx = 0;
    unsigned char key = read_digital_keypad(EDGE);

    if(key == GEAR_UP && idx < 7)   //  Increment Gear
    {
        idx++;
    }
    else if(key == GEAR_DOWN && idx > 0)    //  Decrement Gear
    {
        idx--;
    }
    else if(key == COLLISION)   //  Detect collision
    {
        collision_flag = 1;
        idx = 0;                //  Make gear Neutral
    }

    //  Send Gear in UART

    static unsigned int delay = 0;

    if(delay++ >= 10)
    {
        puts("GEAR : ");
        puts(gear[idx]);
        puts("\r\n");
        
        delay = 0;
    }


    return idx;
}