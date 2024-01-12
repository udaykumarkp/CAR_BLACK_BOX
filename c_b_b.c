 /* 
 NAME         : UDAY KUMAR K P
 DATE         : 21/11/2023                                                   
 TITLE        : CAR_BLACK_BOX                        
 DESCRIPTION  : THIS FILE IS TO MANAGE DASHBOARD LOGIN AND MENU SCREENS
 */                                           

#include "main.h"

unsigned char clock_reg[3],time[9];
unsigned char log[11],sec,return_time;  //INITIALIZING VARIABLES TO READ TIME
extern unsigned char control_flag;      //LINKING THE CONTROL_FLAG OF MAIN FILE
int pos = -1,event = 0;                 //VARIABLES TO FOLLOW THE NO.OF EVENTS
void get_time(void)
{
    clock_reg[0] = read_ds1307(HOUR_ADDR);  // HH -> BCD 
    clock_reg[1] = read_ds1307(MIN_ADDR);   // MM -> BCD 
    clock_reg[2] = read_ds1307(SEC_ADDR);   // SS -> BCD 
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0';  //STORING HOURS TO FIRST 2 INDICES
    time[2] = ':';
    time[3] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time[4] = (clock_reg[1] & 0x0F) + '0';  //STORING MINUTES TO 3,4 INDICES
    time[5] = ':';
    time[6] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time[7] = (clock_reg[2] & 0x0F) + '0';   //STORING SECONDS TO 6,7 INDICES
    time[8] = '\0';
}
void display_dashboard(unsigned char *event,unsigned char speed)
{
    clcd_print("TIME      EV  SP",LINE1(0));    //PRINTING THE DEFAULT LINE ON CLCD
    get_time();                                 //FUNCTION CALL TO GET TIME FROM RTC
    clcd_print(time, LINE2(0)); // HH:MM:SS
    clcd_print(event , LINE2(10));              //PRINTING TIME EVENT AND SPEED IN 2ND LINE
    clcd_putch((speed/10)+'0',LINE2(14));
    clcd_putch((speed%10)+'0',LINE2(15));
}

void log_event_write()                          //FUNCTION TO WRITE A LOG TO EEPROM
{
    unsigned char addr = 0x05;                  //LOG STORING STARTS FROM 0X05
    pos++;
    if(pos == 10)                               //CHECKING THE POSITION TO OVERWRITE THE EXISTED LOG WITH NEW
        pos = 0;
    addr += (pos*10);                           //SETTING THE ADDRESS ACCORDING TO THE POSITION
    if(event < 10 )
        event++;                                //INCREMENTING EVENT TO CHEXK NO OF LOGS ARE STORES
    eeprom_at24C04_str_write(addr, log);        //WRITE DATA TO EEPROM
    
}
void log_event(unsigned char *event,unsigned char speed)
{                                               //FUNCTION TO STORE THE PARTICULAR LOG TO STRING
    get_time();
    log[0] = time[0];
    log[1] = time[1];
    log[2] = time[3];
    log[3] = time[4];
    log[4] = time[6];                           //STORING TIME EVENT AND SPEED IN LOG STRING
    log[5] = time[7];
    log[6] = event[0];
    log[7] = event[1];
    log[8] = (speed/10) + '0';
    log[9] = (speed%10) + '0';
    log[10]= '\0';
    log_event_write();                          //CALLING THE FUNCTION TO STORE LOG DATA THAT IS IN THE STRING
}
void login_screen(void)
{
    clear_display();                            //CLEAR DISPLAY AND PRINT THE DEFAULT LINE
    clcd_print("ENTER THE PASS :",LINE1(0));
    cursor_on(CURS_POS);                        //SWITCH ON THE CURSOR WITH POSITION OF CURSOR AS ARGUMENT
}
char login(unsigned char *reset, unsigned char key) //FUNCTION TO CHECK THE  PASSWORD
{
    static unsigned char attempts = 3 , upassword[4] , spassword[4] , i;
    if(*reset == RESET_PASS)                    //INITIALIZE VARIABLES TO CHECK THE PASSWORD
    {
        upassword[0] = '\0';
        upassword[1] = '\0';
        upassword[2] = '\0';                    //RESET ALL STRING AND KEY FOR THE FIRST ENTRY TO THE FUNCTION
        upassword[3] = '\0';
        i = 0;
        key = 0xFF;
        *reset = RESET_NOTHING;
        return_time = 5;
    }
    if(key == SW4 && i<4)
    {
        return_time = 5;                        //SET TIME TO WAIT IN LOGIN SCREEN
        upassword[i] = '0';                     //PRINT * FOR EVERY CHARACTER THAT IS ENTERED
        clcd_putch('*', LINE2(6+i));            //STORE 0 FOR SW4
        i++;
    }
    else if(key == SW5 && i<4)
    {
        return_time = 5;
        upassword[i] = '1';                     //STORE 1 FOR SW5
        clcd_putch('*' , LINE2(6+i));           //PRINT * CHARACTER
        i++;
    }
    if(return_time == 0)
    {
        cursor_off();    
        clear_display();                        //IF RETURN TIME IS 0 CLEAR DISPLAY
        *reset = RESET_PASS;                     //SET RESET_PASS
        return FAILURE;
    }
    if(i==4 )                                    //IF PASSWORD IS FULLY ENTERED
    {
        for(int j = 0; j< 4 ; j++)
        {
            spassword[j] = eeprom_at24C04_read(j);  //RETRIEVE THE PASSWORD FROM EEPROM
        }
        if(strncmp(upassword,spassword,4) == 0)
        {
            cursor_off();                         //IF PASSWORD MATCHES CLEAR DISPLAY AND OFF CURSOR
            clear_display();
            return SUCCESS;                        //ENTER TO THE MENU
        }
        else
        {
            attempts--;                            //IF PASSWORD IS WRONG DECREMENT ATTEMPTS
            if(attempts == 0)                      //IF ATTEMPTS ARE OVER
            {
                clear_display();
                cursor_off();                      //LOCK THE USER AND DISPLAY A MESSAGE
                clcd_print("YOU ARE LOCKED",LINE1(0));
                clcd_print("WAIT FOR    SEC",LINE2(0));
                sec = 60;
                while(sec)
                {
                    clcd_putch(sec/10 + '0' , LINE2(9));
                    clcd_putch(sec%10 + '0', LINE2(10));
                }
                *reset = RESET_PASS;                //AFTER LOCK-IN PERIOD SET THE _RESET_PASS AND MAKE ATTEMPTS 3
                attempts = 3;
            }
            else
            {
                clear_display();
                clcd_print("WRONG PASS",LINE1(0));      //IF PASSWORD IS WRONG AND ATTEMPTS ARE AVAILABLE
                clcd_putch(attempts + '0' , LINE2(0));
                clcd_print("ATTEMPTS LEFT" , LINE2(2)); //PRINT NO.OF ATTEMPTS AVAILABLE
                __delay_ms(3000);
                *reset = RESET_PASS;                    //SET reset_pass
            }
            login_screen();                             //FUNCTION TO PRINT THE FIRST LINE IN LOGIN SCREEN
        }
    }
}

unsigned char menu_pos=0;                               //VARIABLE TO FOLLOW THE OPTION IN MENU
char display_menu(unsigned char key , unsigned char * reset)
{
    char *menu[] = {"view log","clear log","download log","change pswd","set time"};
    if(*reset == RESET_MENU)                            //PRINT THE MENU ON CLCD
    {
        key = 0xFF;
        menu_pos = 0;                                   //CHECK THE RESET AND MAKE KEY TO OXFF IN FIRST ENTRY
        clear_display();                                //CLEAR DISPLAY TO PRINT THE MENU
        *reset = RESET_NOTHING;
    }
    if(key == SW5 && menu_pos<=3)
    {
        menu_pos++;                                     //IF SWITCH5 IS ENTERED SCROLL DOWN AND CLEAR DISPLAY
        clear_display();
    }
    else if(key == SW4 && menu_pos>0 )
    {
        menu_pos--;                                     //IF SW4 IS ENTERED SCROLL UP AND CLEAR DISPLAY
        clear_display();
    }
    if(menu_pos < 4)
    {
        clcd_print("-> " , LINE1(0));
        clcd_print(menu[menu_pos] , LINE1(3));
        clcd_print(menu[menu_pos+1] , LINE2(3));         //IF MENU_POS IS < 4 PRINTING THE SELECT OPTION(*) AT TOP
    }
    else if(menu_pos == 4)
    {                                                    //ELSE PRINT AT BOTTOM
        clcd_print("-> " , LINE2(0));
        clcd_print(menu[menu_pos-1] , LINE1(3));
        clcd_print(menu[menu_pos] , LINE2(3));            //PRINT THE OTIONS
    } 
    
}
