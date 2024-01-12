 /* 
 NAME         : UDAY KUMAR K P
 DATE         : 21/11/2023                                                  
 TITLE        : CAR_BLACK_BOX                        
 DESCRIPTION  : THIS FILE IS TO READ THE SWITCHES
 */

#include <xc.h>
#include "digital_keypad.h"

void init_digital_keypad(void)
{
    /* Set Keypad Port as input */
    KEYPAD_PORT_DDR = KEYPAD_PORT_DDR | INPUT_LINES; //INITIALIZING SWITCHES
}

unsigned char read_digital_keypad(unsigned char mode)
{                                                   //FUNCTION TO READ SWITCHES
    static unsigned char once = 1;
    
    if (mode == LEVEL_DETECTION)
    {
        return KEYPAD_PORT & INPUT_LINES;           //IF LEVEL MODE IS SELECTED SEND THE SWITCH
    }
    else
    {
        if (((KEYPAD_PORT & INPUT_LINES) != ALL_RELEASED) && once)
        {
            once = 0;
            
            return KEYPAD_PORT & INPUT_LINES;       //IF STATE IS SELECTED SEND SWITCH AND CLEAR FLAG 
        }
        else if ((KEYPAD_PORT & INPUT_LINES) == ALL_RELEASED)
        {
            once = 1;                               //IF SWITCH ARE RELEASED SET THE FLAG
        }
    }
    
    return ALL_RELEASED;
}
