 /* 
 NAME         : UDAY KUMAR K P
 DATE         : 21/11/2023                                                
 TITLE        : CAR_BLACK_BOX                        
 DESCRIPTION  : THIS FILE IS MANAGE OPERATIONS WITH CLCD
 */
#include <xc.h>
#include "clcd.h"

void clcd_write(unsigned char byte, unsigned char mode)  
{                                   //FUNCTION TO WRITE INTO CLCD IN INSTRUCTION OR IN DATA MODE
    CLCD_RS = mode;                 //SELECTING MODE USING RE2 
    
    CLCD_DATA_PORT = byte & 0xF0;   //DATA/INST SENDING THROUGH RD4 - RD7 
    CLCD_EN = HI;
    __delay_us(100);                //SETTING AND CLEARING RE1 AFTER EVERY CALL
    CLCD_EN = LOW;

    CLCD_DATA_PORT = (byte & 0x0F) << 4; //ANOTHER 4 BITS SEND AFTER FIRST 4 BYTES
    CLCD_EN = HI;
    __delay_us(100);                
    CLCD_EN = LOW;
    
    __delay_us(4100);               //DELAY FOR FREE FLOW OF DATA
}

static void init_display_controller(void)
{
    /* Startup Time for the CLCD controller */
    __delay_ms(30);
    
    /* The CLCD Startup Sequence */
    clcd_write(EIGHT_BIT_MODE, INST_MODE);
    __delay_us(4100);
    clcd_write(EIGHT_BIT_MODE, INST_MODE);      //START UP SEQUENCE TO INITIALIZE CLCD
    __delay_us(100);
    clcd_write(EIGHT_BIT_MODE, INST_MODE);
    __delay_us(1); 
    
    clcd_write(FOUR_BIT_MODE, INST_MODE);
    __delay_us(100);
    clcd_write(TWO_LINES_5x8_4_BIT_MODE, INST_MODE);    //SETTING THE CLCD TO 4 BIT ODE
    __delay_us(100);
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(500);                                //SWITCHING ON DISPLAY AND SWITCHING OFF CURSOR
    clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
    __delay_us(100);
}

void init_clcd(void)
{
    /* Setting the CLCD Data Port as Output */
    CLCD_DATA_PORT_DDR = 0x00;
    
    /* Setting the RS and EN lines as Output */
    CLCD_RS_DDR = 0;
    CLCD_EN_DDR = 0;
    
    init_display_controller();
}

void clcd_putch(const char data, unsigned char addr)
{
    clcd_write(addr, INST_MODE);                //FUNCTION TO PRINT CHARACTER ON CLCD
    clcd_write(data, DATA_MODE);
}

void clcd_print(const char *str, unsigned char addr)
{
    clcd_write(addr, INST_MODE);
    
    while (*str != '\0')                         //FUNCTION TO PRINT STRING ON CLCD
    {
        clcd_write(*str, DATA_MODE); 
        str++;
    }
}
void clear_display(void)
{
clcd_write(CLEAR_DISP_SCREEN, INST_MODE);         //FUNCTION TO CLEAR THE CHARACTERS ON DISPLAY
            __delay_us(500);
}
void cursor_on(unsigned char address)
{
    clcd_write(address , INST_MODE);
       __delay_us(100);                           //FUNCTION TO SWITCH ON CURSOR AT REQUIRED POSITION
       clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
       __delay_us(100);
}

void cursor_off(void)
{
       clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);   //FUNCTION TO SWITCH OFF CURSOR
       __delay_us(100);
}