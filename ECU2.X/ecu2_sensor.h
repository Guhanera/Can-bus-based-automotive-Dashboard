#ifndef ECU2_SENSOR_H
#define	ECU2_SENSOR_H

#include <stdint.h>
#include <xc.h>
#include "digital_keypad.h"

#define RPM_ADC_CHANNEL         0x04
#define ENG_TEMP_ADC_CHANNEL    0x06

#define LED_OFF     0
#define LED_ON      1

#define LEFT_INDICATOR      SWITCH1
#define RIGHT_INDICATOR     SWITCH2
#define HAZARD_LIGHT        SWITCH3

#define RIGHT_IND_ON()      (PORTB = PORTB | 0xC0)
#define RIGHT_IND_OFF()     (PORTB = PORTB & ~0xC0)
#define LEFT_IND_ON()       (PORTB = PORTB | 0x03)
#define LEFT_IND_OFF()      (PORTB = PORTB & ~0x03)
#define HAZARD_LIGHT_ON()   (PORTB = PORTB | 0XC3)
#define HAZARD_LIGHT_OFF()  (PORTB = PORTB & ~0XC3)

typedef enum {
    e_ind_off,
    e_ind_left,
    e_ind_right,
    e_hazard
} IndicatorStatus;

extern volatile IndicatorStatus prev_ind_status, cur_ind_status;
extern volatile unsigned char led_state;

uint16_t get_rpm(void);
uint16_t get_engine_temp(void);
IndicatorStatus process_indicator(void);

#endif	/* ECU1_SENSOR_H */