#include "adc.h"
#include "can.h"
#include "ecu1_sensor.h"
#include "msg_id.h"
#include "uart.h"
#include "lcd.h"
#include <builtins.h>
#include <stdint.h>

#define _XTAL_FREQ 20000000

void init_config()
{
    init_adc();
    init_digital_keypad();
    init_can();
    init_uart();
    // init_lcd();
}

unsigned char collision_flag = 0;

int main()
{
    init_config();
    
    uint16_t speed;
    uint8_t speed_data[2] = {0};

    unsigned char gear;
    unsigned int delay = 0;
    
    while(1)
    {
        //  If collision occurs, make Speed = 0 and Gear = Neutral
        if(collision_flag)
        {
            speed = 0;
            speed_data[0] = (speed >> 8) & 0XFF;
            speed_data[1] = speed & 0XFF;
            gear = 0;
            
            can_transmit(SPEED_MSG_ID, speed_data, 2);
            __delay_ms(10);
            can_transmit(GEAR_MSG_ID, &gear, 1);
            __delay_ms(10);  
            can_transmit(COLLISION_MSG_ID,&collision_flag,1);

            continue;

        }

        speed = get_speed();
        gear = get_gear_pos();
        
        speed_data[0] = (speed >> 8) & 0XFF;
        speed_data[1] = speed & 0XFF;
        


        can_transmit(SPEED_MSG_ID, speed_data, 2);
        __delay_ms(10);
        can_transmit(GEAR_MSG_ID, &gear, 1);
        __delay_ms(10);  

    }
}