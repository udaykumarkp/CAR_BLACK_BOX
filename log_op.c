 /* 
 NAME         : UDAY KUMAR K P
 DATE         : 21/11/2023                                                
 TITLE        : CAR_BLACK_BOX                        
 DESCRIPTION  : THIS FILE IS TO VIEW CLEAR AND DOWNLOAD LOGS
 */

#include "main.h"

extern int event,pos;
extern unsigned char control_flag;      //LINKING VARIABLES
char fetch_data[17];    //ARRAY TO FETCH DATA FROM EEPROM

void view_log(unsigned char key ,unsigned char* reset)
{                               //FUNCTION TO VIEW LOGS
    static int pos=0;
    if(*reset == RESET_VIEW)    //RESETTING ALL VALUES BY CHECKING THE RESET VALUE
    {
        key = ALL_RELEASED;
        *reset = RESET_NOTHING; //RESETTING ALL VALUES FOR FIRST ENTRY
        pos = 0;
    }
    if(key == SW5 && event > 0)
    {
        pos = (pos+1)%(event);  //IF SW5 IS PRESSED MOVE TO THE NEXT LOG
    }
    else if(event == 0)
    {
        clear_display();        //IF THERE ARE NO EVENTS
        clcd_print("NO LOGS.....",LINE1(0));
        __delay_ms(3000);
        clear_display();        //PRINT THERE ARE NO LOGS AND CLEAR THE SCREEN
        reset = RESET_MENU;
        control_flag = MENU;    //MOVE BACK TO MENU
        return;
    }
    fetch_data_from_memory(pos);       //IF THE LOGS ARE PRESENT FETCH DATA FROM EEPROM
    clcd_print(fetch_data , LINE2(0)); //PRINT THE DATA ON THE CLCD
}
void fetch_data_from_memory(unsigned char i)
{                                       //FUNCTION TO FETCH DATA FROM EEPROM
    unsigned char addr = 0x05 + i*10;
    fetch_data[0] = i+'a';              //SERIAL NUMBER AS ALPHABETS
    fetch_data[1] = '.';
    fetch_data[2] = eeprom_at24C04_read(addr);      //HOURS FETCHING
    fetch_data[3] = eeprom_at24C04_read(addr+1);
    fetch_data[4] = ':';
    fetch_data[5] = eeprom_at24C04_read(addr+2);    //MINUTES FETCHING
    fetch_data[6] = eeprom_at24C04_read(addr+3);
    fetch_data[7] = ':';
    fetch_data[8] = eeprom_at24C04_read(addr+4);    //SECONDS FETCHING
    fetch_data[9] = eeprom_at24C04_read(addr+5);
    fetch_data[10] = ' ';
    fetch_data[11] = eeprom_at24C04_read(addr+6);
    fetch_data[12] = eeprom_at24C04_read(addr+7);   //EVENT FETCHING
    fetch_data[13] = ' ';
    fetch_data[14] = eeprom_at24C04_read(addr+8);
    fetch_data[15] = eeprom_at24C04_read(addr+9);   //SPEED FETCHING
    fetch_data[16] = '\0';                          //STRING END
}

void clear_log(void)
{                                               //FUNCTION TO CLEAR LOGS
    for(int i=0;i<100;i++)
    eeprom_at24C04_byte_write(0x05 + i , 0xFF); //WRITE ALL LOGS WITH 1'S
    event = 0;
    pos = -1;                                   //SET EVENT AND POSITION TO FOLLLOW THE LOGS
    clear_display();                            //CLEAR DISPLAY AND GO TO MENU
}

void download_log(void)                         //FUNCTION TO DOWNLOAD LOGS USING UART
{
    if(event == 0)
    {                                           //IF LOGS AR NOT PRESENT
        clear_display();
        clcd_print("NO LOGS TO",LINE1(0));      //PRINT NO LOGS TO DOWNLOAD AND GO BACK TO MENU
        clcd_print("DOWNLOAD",LINE2(0));
        __delay_ms(2000);                       //DELAY TO SHOW THE PRINTED LINE
        clear_display();
        return;
    }
    for(int i=0; i< event ; i++ )
    {
       fetch_data_from_memory(i);
       puts(fetch_data);                        //IF LOGS ARE PRESENT
       puts ("  ");
       //FETCH DATA FROM EEPROM AND SEND THROUGH THE UART
    }
}