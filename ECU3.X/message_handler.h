#ifndef MESSAGE_HANDLER_H
#define	MESSAGE_HANDLER_H

#include <stdint.h>

#define LED_OFF     0
#define LED_ON      1

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

void process_canbus_data(void);
void update_indicator_display(void);

void handle_speed_data(uint8_t *data, uint8_t len);
void handle_gear_data(uint8_t *data, uint8_t len);
void handle_rpm_data(uint8_t *data, uint8_t len);
void handle_engine_temp_data(uint8_t *data, uint8_t len);
void handle_indicator_data(uint8_t *data, uint8_t len);

#endif	/* MESSAGE_HANDLER_H */