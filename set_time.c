 /* 
 NAME         : UDAY KUMAR K P
 DATE         : 21/11/2023                                                  
 TITLE        : CAR_BLACK_BOX                        
 DESCRIPTION  : THIS FILE IS TO EDIT TIME
 */

#include "main.h"

unsigned char set_tm[9];            //VARIABLE TO SET TIME

void set_time(unsigned char key , unsigned char * reset)
{
    static int i = 0 , curs_flag ;      //VARIABLES TO KEEP TRACK OF TIME
    if(*reset == RESET_TIME)
    {
        for(int i=0;i<8;i++)
            set_tm[i] = '0';            //RESET VARIABLES FOR FIRST ENTRY
        i = 0;
        key = ALL_RELEASED;
        *reset = RESET_NOTHING;
        set_tm[2] = ':';
        set_tm[5] = ':';
        set_tm[8] = '\0';
    }
    if(key == SW4 && i < 7)
    {
       i++;
       if(i==2 || i==5)                 //IF SW4 IS PRESSED MOVE THE CURSOR
           i++;
    }
    else if(key == SW5)
    {
        switch(i)                        //IF SW5 IS PRESSED INCREMENT THE SPECIFIC OPTIONS
        {
            case 0: 
                set_tm[0] = '0' + (set_tm[0]-'0'+1)% 3;
                break;
            case 1:                         //CASE TO INCREMENT HOURS
                if(set_tm[0] == '2')
                    set_tm[1] = '0' + (set_tm[1]-'0'+1) % 4;
                else
                    set_tm[1] = '0' + (set_tm[1]-'0'+1)%10;
                break;
            case 3: 
                set_tm[3] = '0' + (set_tm[3]-'0'+1)%6;
                break;
            case 4:                             //CASE TO INCREMENT MINUTES
                set_tm[4] = '0' + (set_tm[4]-'0'+1)%10;
                break;
            case 6: 
                set_tm[6] = '0' + (set_tm[6]-'0'+1)%6;
                break;
            case 7:                             //CASE TO INCREMENT SECONDS
                set_tm[7] = '0' + (set_tm[7]-'0'+1)%10;
                break;
        }
    }
    if(set_tm[0] == '2')
          set_tm[1] = '0' + (set_tm[1]-'0') % 4;    //IF FIST DIGIT OF HOURS IS 2 SET THE SECOND DIGIT LESS THAN 4
    clcd_print(set_tm,LINE2(0));
    if(curs_flag = (!curs_flag))                    //PRINT TIME AND CURSOR
        clcd_putch(0xFF , LINE2(i));
}

void save_time(void)
{
    unsigned char data = 0;
    data = data| ((set_tm[0]-'0')<<4) | (set_tm[1] - '0');
    write_ds1307(HOUR_ADDR,data);                   //WRITING HOURS TO RTC
    data = 0;
    data = data| ((set_tm[3]-'0')<<4) | (set_tm[4] - '0');
    write_ds1307(MIN_ADDR,data);                    //WRITING MINUTES TO RTC
    data = 0;
    data = data| ((set_tm[6]-'0')<<4) | (set_tm[7] - '0');
    write_ds1307(SEC_ADDR,data);                    //WRITING SECONDS TO RTC
}
