 /* 
 NAME         : UDAY KUMAR K P
 DATE         : 21/11/2023                                            
 TITLE        : CAR_BLACK_BOX                        
 DESCRIPTION  : THIS FUNCTION IS TO READ AND WRITE FROM RTC
 */

#include <xc.h>
#include "i2c.h"
#include "ds1307.h"

void init_ds1307(void)                  //FUNCTION TO INITIALIZE DS1307
{
    unsigned char dummy;
    dummy = read_ds1307(SEC_ADDR);      //SEQUENCE CODE FROM DATA SHEET TO SWITCH ON DS1307
    dummy = dummy & 0x7F;
    write_ds1307(SEC_ADDR, dummy);
}

unsigned char read_ds1307(unsigned char addr)
{
    unsigned char data;                 //READING FROM RTC THROUGH I2C PROTOCOL
    
    i2c_start();
    i2c_write(SLAVE_WRITE);
    i2c_write(addr);
    i2c_rep_start();                    //START+ RTC ADDRESS/WRITE + REGISTER ADDRESS + REP_START + RTC ADDRESS/READ
    i2c_write(SLAVE_READ);              //READ DATA AND STOP
    data = i2c_read(0);
    i2c_stop();
    
    return data;
}

void write_ds1307(unsigned char addr, unsigned char data) // SEc_ADDR, data
{
    i2c_start();                            //FUNCTION TO WRITE INTO RTC
    i2c_write(SLAVE_WRITE);
    i2c_write(addr);                        //START + RTC ADDRESS/WRITE + REGISTER ADDRESS + DATA TO WRITE + STOP
    i2c_write(data);
    i2c_stop();
}
