#include "ecu2_sensor.h"
#include "adc.h"
#include "can.h"
#include "digital_keypad.h"
#include "msg_id.h"
#include "uart.h"
#include "lcd.h"
#include <stdint.h>

#define _XTAL_FREQ 20000000

void init_config()
{
    TRISB = TRISB & 0X3C;
    init_adc();
    init_digital_keypad();
    init_uart();
    init_can();
}

int main()
{
    init_config();

    uint16_t rpm;
    uint8_t rpm_arr[2];

    uint16_t engine_temp;
    uint8_t engine_temp_arr[2];
    
    IndicatorStatus indicator;
    
    uint16_t msg_id;
    uint8_t data[8];
    uint8_t len;
    
    unsigned char collision_flag = 0;

    while(1)
    {
        can_receive(&msg_id, data, &len);
        
        //  Check if collision occured

        if(len != 0 && msg_id == COLLISION_MSG_ID)
        {   
            collision_flag = 1;
        }
        
        //  If Collision -> Make RPM = 0
        
        if(collision_flag)
        {
            rpm = 0;
            rpm_arr[0] = (rpm >> 8) & 0XFF;
            rpm_arr[1] = rpm & 0XFF;
            
            indicator = e_hazard;
            
            can_transmit(RPM_MSG_ID, rpm_arr, 2);
            __delay_ms(10);
            
            can_transmit(INDICATOR_MSG_ID, (uint8_t*)&indicator, 1);
            __delay_ms(10);
            
            continue;
            
        }
        
        //-------------- Get Indicator status ---------------
   
        indicator = process_indicator();
        

        //----------- Get RPM --------------------------------

        rpm = get_rpm();

        rpm_arr[0] = (rpm >> 8) & 0XFF;
        rpm_arr[1] = rpm & 0XFF;

        //------------ Get Engine temperature ---------------

        engine_temp = get_engine_temp();
        
        engine_temp_arr[0] = (engine_temp >> 8) & 0XFF;
        engine_temp_arr[1] = engine_temp & 0XFF;

        //---------------------------------------------------

        can_transmit(RPM_MSG_ID, rpm_arr, 2);
        __delay_ms(10);
        can_transmit(ENG_TEMP_MSG_ID, engine_temp_arr, 2);
        __delay_ms(10);
        can_transmit(INDICATOR_MSG_ID, (uint8_t*)&indicator, 1);
        __delay_ms(10);
    }
}