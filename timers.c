 /* 
 NAME         : UDAY KUMAR K P
 DATE         : 21/11/2023                                                  
 TITLE        : CAR_BLACK_BOX                        
 DESCRIPTION  : THIS FILE IS TO INITIALIZE TIMER2

#include <xc.h>

void init_timer2(void)
{
    /* Selecting the scale as 1:16 */
    T2CKPS0 = 1;
    T2CKPS1 = 1;

    /* Loading the Pre Load register with 250 */
    PR2 = 6;
    
    /* The timer interrupt is enabled */
    TMR2IE = 1;
       
    /* Switching on the Timer2 */
    TMR2ON = 1;
}