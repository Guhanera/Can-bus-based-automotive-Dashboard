#include <xc.h>
#include <stdint.h>
#include "can.h"
#include "lcd.h"
#include "msg_id.h"
#include "message_handler.h"
#include "isr.h"
#include "timer2.h"
#include "uart.h"

static void init_leds()
{
    TRISB = TRISB & 0X3C;
    PORTB = 0x00;
}

static void init_config(void)
{
    init_can();
    init_tim2();
    init_uart();
    init_lcd();
    init_leds();
}

void main(void)
{
    // Initialize peripherals
    init_config();

    lcd_putstr("SPD G RPM  TMP I", 0, 0);

    while (1)
    {
        process_canbus_data();
        update_indicator_display();
    }
}