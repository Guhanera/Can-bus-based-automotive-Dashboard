#include "ecu2_sensor.h"
#include "adc.h"
#include "can.h"
#include "digital_keypad.h"
#include "lcd.h"
#include "msg_id.h"
#include "uart.h"
#include <stdint.h>

#define MAX_RPM 6000UL

uint16_t get_rpm()
{
    uint32_t sum = 0;
    
    //  Averaging ADC values and get RPM

    for(unsigned char i = 0; i < 16; i++)
    {
        sum += read_adc(RPM_ADC_CHANNEL);
    }

    uint16_t pot_val = sum / 16;

    uint16_t rpm = (uint16_t)((uint32_t)pot_val * MAX_RPM / 1023UL);

    
    //  Format as "RPM : XXXX" to send in UART
    
    char rpm_str[12] = "RPM : ";
    unsigned int temp = rpm;
    int digits = 0;

    do
    {
        digits++;
        temp /= 10;
    } while(temp);

    temp = rpm;

    int idx = 6 + digits - 1;

    do
    {
        rpm_str[idx--] = (temp % 10) + '0';
        temp /= 10;
    } while(temp);

    rpm_str[6 + digits] = '\0';

    //  Send RPM data in UART
    
    static unsigned int delay = 0;

    if(delay++ >= 10)
    {
        puts(rpm_str);
        puts("\r\n");
        delay = 0;
    }

    return rpm;
}

uint16_t get_engine_temp()
{
    //  Get temperature value
    
    unsigned int temp_adc_val = read_adc(ENG_TEMP_ADC_CHANNEL);
    uint16_t temperature = (uint16_t)((uint32_t)temp_adc_val * 500UL / 1023UL);
    
    if(temperature >= 99)
        temperature = 99;

    //  Format as "TEMP : XXX" to send in UART
    
    char temperature_str[10] = "TEMP : ";
    unsigned int temp = temperature;
    int digits = 0;

    do
    {
        digits++;
        temp/=10;
    }while(temp);

    temp = temperature;

    int idx = 7 + digits - 1;

    do
    {
        temperature_str[idx--] = (temp % 10) + '0';
        temp/=10;
    }while(temp);

    temperature_str[7 + digits] = '\0';

 
    //  Send Engine Temperature in UART

    static unsigned int delay = 0;

    if(delay++ >= 10)
    {
        puts(temperature_str);
        puts("\r\n");

        delay = 0;
    }

    return temperature;
}

IndicatorStatus process_indicator(void)
{
    //  Maintain the status of all indicators
    
    static uint8_t left_indi_status  = 0;
    static uint8_t right_indi_status = 0;
    static uint8_t hazard_status     = 0;

    unsigned char key = read_digital_keypad(EDGE);

    /* Handle key press */
    
    if(key == LEFT_INDICATOR)
    {
        left_indi_status  = !left_indi_status;

        if(left_indi_status)
        {
            right_indi_status = 0;
            hazard_status     = 0;
        }
    }
    else if(key == RIGHT_INDICATOR)
    {
        right_indi_status = !right_indi_status;

        if(right_indi_status)
        {
            left_indi_status = 0;
            hazard_status    = 0;
        }
    }
    else if(key == HAZARD_LIGHT)
    {
        hazard_status = !hazard_status;

        if(hazard_status)
        {
            left_indi_status  = 0;
            right_indi_status = 0;
        }
    }

    /* Return current indicator status and send in UART */

    if(left_indi_status)
    {
        puts("Left\r\n");
        return e_ind_left;
    }

    if(right_indi_status)
    {
        puts("Right\r\n");
        return e_ind_right;
    }

    if(hazard_status)
    {
        puts("Hazard\r\n");
        return e_hazard;
    }

    return e_ind_off;
}