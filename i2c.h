/* 
 * File:   i2c.h
 */

#ifndef I2C_H
#define	I2C_H

#define FOSC            20000000

//FUNCTION TO INITIALIZE I2C BLOCK
void init_i2c(unsigned long baud);
//FUNCTION TO SEND START BIT
void i2c_start(void);
//FUNCTION FOR REPEAT START
void i2c_rep_start(void);
//FUNCTION TO SEND STOP BIT
void i2c_stop(void);
//FUNCTION TO READ DATA FROM SSPBUF AFTER RECIEVING
unsigned char i2c_read(unsigned char ack);
//FUNCTION TO SEND DATA THROUGH I2C
int i2c_write(unsigned char data);

#endif	/* I2C_H */

