#include "timer2.h"

void init_tim2(void)
{
    PR2 = 249;

    //  POST SCALAR -> 1:10
    T2OUTPS3 = 1;
    T2OUTPS2 = 0;
    T2OUTPS1 = 0;
    T2OUTPS0 = 1;

    //  PRESCALAR -> 1:16
    T2CKPS1 = 1;
    T2CKPS0 = 1;

    TMR2ON = 1;

    //  Enable Interrupts
    GIE = 1;
    PEIE = 1;
    TMR2IE = 1;
    TMR2IF = 0;
}