# 1 "EEprom.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.45\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "EEprom.c" 2
# 10 "EEprom.c"
# 1 "./EEprom.h" 1
# 12 "./EEprom.h"
unsigned char eeprom_at24C04_read(unsigned char addr);

void eeprom_at24C04_byte_write(unsigned char addr, char data);

void eeprom_at24C04_str_write(unsigned char addr, unsigned char *str);
# 10 "EEprom.c" 2

# 1 "./i2c.h" 1
# 11 "./i2c.h"
void init_i2c(unsigned long baud);

void i2c_start(void);

void i2c_rep_start(void);

void i2c_stop(void);

unsigned char i2c_read(unsigned char ack);

int i2c_write(unsigned char data);
# 11 "EEprom.c" 2


unsigned char eeprom_at24C04_read(unsigned char addr)
{
    unsigned char data;

    i2c_start();
    i2c_write(0b10100000);
    i2c_write(addr);
    i2c_rep_start();
    i2c_write(0b10100001);
    data = i2c_read(0);
    i2c_stop();

    return data;

}

void eeprom_at24C04_byte_write(unsigned char addr, char data)
{
    i2c_start();
    i2c_write(0b10100000);
    i2c_write(addr);
    i2c_write(data);
    i2c_stop();
}

void eeprom_at24C04_str_write(unsigned char addr, unsigned char *str)
{
    unsigned char i = 0;
    while(str[i] != '\0')
    {
      eeprom_at24C04_byte_write(addr+i, str[i]);
      i++;
    }
}
