#include <xc.h>
#include <string.h>
#include "message_handler.h"
#include "msg_id.h"
#include "can.h"
#include "lcd.h"
#include "uart.h"

extern unsigned char sec_flag;

static uint8_t left_indi_status  = 0;
static uint8_t right_indi_status = 0;
static uint8_t hazard_status     = 0;

char gear[] = {'N','1','2','3','4','5','6','R'};

// itoa function

void my_itoa(unsigned int num, char *str)
{
    if(num == 0)
    {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    int digits = 0;
    unsigned int temp = num;

    while(temp > 0)
    {
        digits++;
        temp /= 10;
    }

    temp = num;
    str[digits] = '\0';

    while(temp)
    {
        str[--digits] = (temp % 10) + '0';
        temp /= 10;
    }
}

void handle_speed_data(uint8_t *data, uint8_t len)
{
    uint16_t speed;

    speed = ((uint16_t)data[0] << 8) | data[1];

    char speed_str[6];

    my_itoa(speed, speed_str);  //  Convert to string

    if(speed < 9)
    {
        lcd_putstr("  ",1,1);
    }
    else if(speed < 99)
    {
        lcd_putstr(" ",1,2);
    }
    
    lcd_putstr(speed_str, 1, 0);    //  Display Speed
}

void handle_gear_data(uint8_t *data, uint8_t len)
{
    uint8_t gear_pos = data[0];

    //  Display Gear position
    
    if(gear_pos < sizeof(gear))
    {
        lcd_putch(gear[gear_pos], 1, 4);
    }
}

void handle_rpm_data(uint8_t *data, uint8_t len)
{
    uint16_t rpm;

    rpm = ((uint16_t)data[0] << 8) | data[1];

    char rpm_str[6];

    my_itoa(rpm, rpm_str);  //  Convert RPM value to string
    
    if(rpm < 9)
    {
        lcd_putstr("   ",1,7);
    }
    else if(rpm < 99)
    {
        lcd_putstr("  ",1,8);
    }
    else if(rpm < 999)
    {
        lcd_putstr(" ",1,9);
    }
    
    lcd_putstr(rpm_str, 1, 6);  //  Display RPM
}

void handle_engine_temp_data(uint8_t *data, uint8_t len)
{
    uint16_t temp;

    temp = ((uint16_t)data[0] << 8) | data[1];

    char temp_str[6];

    my_itoa(temp, temp_str);    //  Convert temperature to string

    if(temp > 9)
    {
        lcd_putstr("  ",1,12);
    }

    lcd_putstr(temp_str, 1, 11);    //  Display temperature
}

void handle_indicator_data(uint8_t *data, uint8_t len)
{
    uint8_t indi = data[0];

    switch(indi)
    {
        case e_ind_left:                //  Left Indicator
            left_indi_status  = 1;
            right_indi_status = 0;
            hazard_status     = 0;
            break;

        case e_ind_right:               //  Right indicator
            right_indi_status = 1;
            left_indi_status  = 0;
            hazard_status     = 0;
            break;

        case e_hazard:                  //  Hazard 
            hazard_status     = 1;
            left_indi_status  = 0;
            right_indi_status = 0;
            break;

        case e_ind_off:                 //  Indicator Off
            left_indi_status  = 0;
            right_indi_status = 0;
            hazard_status     = 0;
            break;
    }
}

void update_indicator_display(void)
{
    if(left_indi_status)    //  Blink Left indicator
    {
        lcd_putch(0X7F, 1, 15);

        RIGHT_IND_OFF();

        if(sec_flag)
            LEFT_IND_ON();
        else
            LEFT_IND_OFF();
    }
    else if(right_indi_status)      //  Blink Right Indicator
    {
        lcd_putch(0X7E, 1, 15);

        LEFT_IND_OFF();

        if(sec_flag)
            RIGHT_IND_ON();
        else
            RIGHT_IND_OFF();
    }
    else if(hazard_status)      //  Blink both LEft and Right indicator
    {
        lcd_putch('H', 1, 15);

        if(sec_flag)
            HAZARD_LIGHT_ON();
        else
            HAZARD_LIGHT_OFF();
    }
    else                        //  Turn off all indicators
    {
        LEFT_IND_OFF();
        RIGHT_IND_OFF();
        HAZARD_LIGHT_OFF();

        lcd_putch(' ', 1, 15);
    }
}

void process_canbus_data(void)
{
    uint16_t msg_id;
    uint8_t data[8];
    uint8_t len;

    can_receive(&msg_id, data, &len);   //  Receive CAN frames

    if(len == 0)
        return;

    //  Process CAN frames
    
    switch(msg_id)
    {
        case SPEED_MSG_ID:
            handle_speed_data(data, len);
            break;

        case GEAR_MSG_ID:
            handle_gear_data(data, len);
            break;

        case RPM_MSG_ID:
            handle_rpm_data(data, len);
            break;

        case ENG_TEMP_MSG_ID:
            handle_engine_temp_data(data, len);
            break;

        case INDICATOR_MSG_ID:
            handle_indicator_data(data, len);
            break;
            
        default:
            break;
    }
}