#include "isr.h"

volatile unsigned char sec_flag = 0;

void __interrupt() isr()
{
    static unsigned int count = 0;
    if(TMR2IF)
    {
        TMR2IF = 0;
        count++;

        if(count >= 125)    //  1 second timeout
        {
            sec_flag = !sec_flag;
            count = 0;
        }
    }
}