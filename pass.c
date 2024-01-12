 /* 
 NAME         : UDAY KUMAR K P
 DATE         : 21/11/2023                                                  
 TITLE        : CAR_BLACK_BOX                        
 DESCRIPTION  : THIS FILE IS TO CHANGE THE PASSWORD
 */

#include "main.h"

extern unsigned char control_flag;              //LINKING VARIABLES
void change_pass(unsigned char key ,unsigned char * reset)
{                                                //FUNCTION TO CHANGE PASSWORD
   static char i = 0,new_pass[4],re_pass[4],clear_flag = 0;
   if(*reset == RESET_CHANGE)
   {
       for(int i = 0;i<4;i++)
       {
           new_pass[i]  = '\0';                 //CLEAR ALL VARIABLES FOR THE FIRST ENTRY TO THE F8NCTION
           re_pass[i]   = '\0';
       }
       key = ALL_RELEASED;
       clear_flag = 1;
       i=0;
       *reset = RESET_NOTHING;
       cursor_on(CURS_POS);                     //SWITCHING ON THE CURSOR
   }
   if(key == SW4)
   {
       if(i<4)
           new_pass[i] = '0';
       else                                      //READING 0 IF SW4 IS PRESSED
           re_pass[i%4] = '0';
       clcd_putch('*',0xC6+(i%4));               //PRINTING *CHARACTER
       i++;
   }
   if(key == SW5)
   {
       if(i<4)
           new_pass[i] = '1';
       else                                         //READING 1 IF SW5 IS PRESSED
           re_pass[i%4] = '1';
       clcd_putch('*',0xC6+(i%4));                  //PRINTING * CHARACTER
       i++;
   }
   if(i==4 && clear_flag)
   {
       clear_flag = !clear_flag;
       clear_display();                                 //IF 4 CHARACTERS ARE READ
       clcd_print("RE-ENTER PASS :" , LINE1(0));        //THEN READ AGAIN 
       cursor_off();
       cursor_on(CURS_POS);                             //SWITCH ON THE CURSOR
   }
   if(i==8)
   {
       clear_display();
       cursor_off();                                    //IF BOTH PASSWORDS ARE READ
       if(strncmp(new_pass,re_pass,4) == 0)             //CHECK IF BOTH ARE SAME
       {
           for(int i = 0 ; i<4 ; i++)
               eeprom_at24C04_byte_write( i , new_pass[i]);         
           clcd_print("PASSWORD CHANGE",LINE1(0));      //IF BOTH ARE SAME STORE TO THE EEPROM
           clcd_print("SUCCESS!!",LINE2(0));
           __delay_ms(2000);
           control_flag = DASH_BOARD;                   //HEAD BACK TO DASHBOARD
       }
       else
       {
           clcd_print("NOT MATCHED..",LINE1(0));        //IF PASSWORD DOESNT MATCH
           __delay_ms(2000);
           reset = RESET_MENU;                          //HEAD MACK TO MENU
           control_flag = MENU;
       }
       clear_display();                                 //CLEAR DISPLAY
   }
}
