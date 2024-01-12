 /* 
 NAME         : UDAY KUMAR K P
 DATE         : 21/11/2023                                                  
 TITLE        : CAR_BLACK_BOX                        
 DESCRIPTION  : THIS FILE IS TO PERFORM I2C COMMUNICATION PROTOCOL
 */

#include <xc.h>
#include "i2c.h"

void init_i2c(unsigned long baud)  //FUNCTION TO INITIALIZE I2C BLOCK
{
    /* Set I2C Master Mode */
    SSPM3 = 1;
    
    /* Set the Required Baudrate */
    SSPADD  = (unsigned char)(FOSC / (4 * baud)) - 1;
    
    /* Enable SSP */
    SSPEN = 1;
}

static void i2c_wait_for_idle(void)
{
    /* Wait till no activity on the bus */
    while (R_nW || (SSPCON2 & 0x1F)); // R_nW || (SSPCON2 & 0x1F) == 0
}

void i2c_start(void)
{
    i2c_wait_for_idle();            //WAIT UNTIL THE DATA LINE IS IDLE AND SET SEN TO SEND START SIGNAL
    SEN = 1;
}

void i2c_rep_start(void)
{
    i2c_stop();                     //CALL START AND STOP FOR REPEAT START
    i2c_start();
    
}

void i2c_stop(void)
{
    i2c_wait_for_idle();            //WAIT FOR DATA LINE AND SET PEN TO SEND STOP SIGNAL
    PEN = 1;
}

unsigned char i2c_read(unsigned char ack)
{                                   //FUNCTION TO READ DATA
    unsigned char data;
    
    i2c_wait_for_idle();            //WAIT FOR DATA LINE
    RCEN = 1;                       //SET RCEN 
    
    i2c_wait_for_idle();
    data = SSPBUF;                  //READ DATA FROM SSPBUF
    
    if (ack == 1)
    {
        ACKDT = 1;
    }
    else                            //IF ACK = 0 SET ACKNOWLEDGEMENT BIT TO 0
    {
        ACKDT = 0;
    }
  
    ACKEN = 1;                      //SET ACKEN TO SEND ACKNOWLEDGEMENT BIT
    
    return data;                    //SEND THE DATA THATS READ
}

int i2c_write(unsigned char data)
{
    i2c_wait_for_idle();
    SSPBUF = data;                  //WAIT FOR DATA LINE
                                    //LOAD DATA TO SSPBUF
    return !ACKSTAT; // !1
}