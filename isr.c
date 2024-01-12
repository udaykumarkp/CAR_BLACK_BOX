 /* 
 NAME         : UDAY KUMAR K P
 DATE         : 21/11/2023                                                  
 TITLE        : CAR_BLACK_BOX                        
 DESCRIPTION  : THIS FILE IS TO HANDLE INTERRUPTS
 */


#include <xc.h>
#include "main.h"
extern unsigned char sec,return_time;           //LINKING VARIABLES THAT ARE IN OTHER FILES
void __interrupt() isr(void)
{
    static unsigned int count = 0;              //COUNT TO CLAUCLATE 1 SECOND
    
    if (TMR2IF == 1)
    {
        TMR2 = TMR2+8;                          //RESETTING TMR2 VALUES WITH RESPECT TO INTERRUPT LATENCY
        if (++count == 1250)
        {
            count = 0;
            if(sec > 0)                         //RESETIING COUNT VALUE AND MANAGING SEC WHEN IT IS IN LOGIN SCREEN
                sec-- ;
            if(sec == 0 && return_time >0)
                return_time--;                  //RETURN_TIME SETTING TO COME BACK FROM LOGIN SCREEN
        }
        
        TMR2IF = 0;                             //CLEARING THE INTERRUPT FLAG
    }
}