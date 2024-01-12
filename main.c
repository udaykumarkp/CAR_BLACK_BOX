 /* 
 NAME         : UDAY KUMAR K P
 DATE         : 21/11/2023                                                  
 TITLE        : CAR_BLACK_BOX                        
 DESCRIPTION  : THIS FILE IS TO PERFORM MAIN FUNCTION, TO CONTROl SCREEN AND INPUTS 
 */


#include "main.h"
#pragma config WDTE = OFF 
extern unsigned char menu_pos;      //VARIABLES TO FOLLOW MENU POSITION AND SCREEN
unsigned char control_flag = 0x01;
void init()
{
    init_digital_keypad();
    init_adc();
    init_clcd();            //INITIALIZING ALL PERIPHERALS THAT WE REQUIRE
    init_i2c(100000);
    init_ds1307();
    init_timer2();
    init_uart(9600);
    GIE = 1;                //ENABLING INTERRUPTS
    PEIE = 1;
}

void main(void) {
    init();
    unsigned char event[3] = "ON";                       //VAR TO READ GEARS
    unsigned char speed = 0,  reset = RESET_PASS;        //VAR TO READ SPPED AND TO FOLLOW THE FIRST ENTRY TO FUNCTION
    unsigned char key= ALL_RELEASED , pre_key = ALL_RELEASED , press_duration = 0,store;
    char gr = 0;                                         //VAR TO READ SWITCHES AND TO FOLLOW GEAR
    char *gear[] = {"C ","G1","G2","G3","G4","G5","R "}; //GEARS AS STRINGS TO DISPLAY IN CLCD
    log_event(event,speed);                              //STORING THE FIRST EVENT OF THE CAR
    eeprom_at24C04_str_write(0x00,"0000");               //STORING PASSWORD TO EEPROM
    TMR2ON = 0;                                          //SWITCHING OFF TIMER
    while(1)
    {
        speed = read_adc()/10;                  //READING SPEED
        key = read_digital_keypad(LEVEL);       //READING SWITCHES
        if(key != ALL_RELEASED)
        {
            store = key;                        //DETECTING SHORT OR LONG PRESS
            press_duration++;
        }
        else
        {
            pre_key = store;                    //STORING SWITCH AFTER RELEASING
            store = ALL_RELEASED;
        }
        for(int i =0;++i<3000;);
        if(speed > 99)
            speed = 99;                         //CHECKING SPEED IF GREATER THAN 99
        if(key == ALL_RELEASED)                 //AFTER RELEASING SWITCH
        {
        if(pre_key == SW1)
        {
            gr=0;
            strcpy(event,gear[gr]);             //IF SW1 IS PRESSED , CONSIDERING IT AS COLLISION
            log_event(event,speed);             //STORING DATA TO EEPROM
        }
        else if(pre_key == SW2 && gr<6)
        {
            strcpy(event,gear[++gr]);           //IF SW2 IS PRESSED INCREMENT THE GEAR UPTO REVERSE GEAR
            log_event(event,speed);             //STORE LOG DATA
        }
        else if(pre_key == SW3 && gr>1)
        {
            strcpy(event,gear[--gr]);           //IF SW3 IS PRESSED DECREMENT THE GEAR UNTIL GEAR1
            log_event(event,speed);             //STORE LOG DATA
        }
        else if((pre_key == SW4 || pre_key == SW5) && control_flag == DASH_BOARD )
        {
            TMR2ON = 1;                     //IF WE ENTER LOGIN SCREEN SWITCH ON TIMER
            control_flag = LOGIN_SCREEN;
            login_screen();                 //PRINT THE LOGIN SCREEN DEFAULT CODE ON CLCD
            reset = RESET_PASS;             //SETTING RESET FOR ITS FIRST ENTRY DURING SWITCH PRESS
        }
        else if(pre_key == SW4 && press_duration > 5)
        {
            if(control_flag == VIEW_LOG )
            {
            clear_display();                //IF SW4 IS LONG PRESSED RETURN BACK FROM THE CURRENT SCREEN
            control_flag = MENU ;
            reset = RESET_MENU;             //IF WE ARE IN VIEW LOG SCREEN RETURN BACK TO MENU
            }
            else if(control_flag == MENU)
            {
                clear_display();            //IF WE ARE IN MENU SCREEN RETURN TO DASHBOARD SCREEN
                control_flag = DASH_BOARD;
            }
            else if(control_flag == SET_TIME)
            {
                save_time();                  //IF WE ARE IN SET TIME SCREEN RETURN TO DASHBOARD SCREEN
                clear_display();
                control_flag = DASH_BOARD;
            }
        }
        else if(pre_key == SW5 && control_flag == MENU && press_duration > 5)
        {
            switch(menu_pos)
            {                                   //IF SWITCH5 IS LONG PRESSED ENTER TO SUBSCREEN
                case 0:
                    clear_display();
                    control_flag = VIEW_LOG;    //FOW VIEWING LOGS SELCT VIEWLOG OPTION WITH *
                    reset = RESET_VIEW;
                    clcd_print("SL  TIME   EV SP",LINE1(0));    //PRINTING THE DEFAULT LINE
                    break;
                case 1:
                    clear_display();
                    control_flag = CLEAR_LOG;       //SETTING CONTROL FLAG TO CLEAR THE LOGS THAT ARE SAVED
                    clcd_print("PREV MENU -> SW4" , LINE1(0));
                    clcd_print("CLEAR LOG -> SW5" , LINE2(0));
                    break;                          //PRINTING THE OPTIONS TO SELECT
                case 2 :
                    control_flag = DOWNLOAD_LOG;
                    clear_display();                //SETTING CONTROL FLAG TO DOWNLOAD LOGS
                    clcd_print("DOWNLOADING....." , LINE1(0));
                    __delay_ms(1500);               //PRINTING THE LINE TO DISPLAY THE LOGS ARE BEEN DOWNLOADING
                    break;
                case 3 :
                    control_flag = CHANGE_PASS;
                    clear_display();                //SETTING THE CONTROL TO CHANGE PASS
                    clcd_print("ENTER NEW PASS :" , LINE1(0));
                    reset = RESET_CHANGE;           //SETTING RESET FOR THE FIRST ENTRY OF THE FUNCTION
                    break;
                case 4:
                    control_flag = SET_TIME;        //SETTING CONTROL FLAG TO SET TIME
                    clear_display();
                    clcd_print("ENTER THE TIME" , LINE1(0));
                    reset = RESET_TIME;             //SETTING RESET FOR THE FIRST ENTRY OF THE FUNCTION
                    break;
            }         
        }
        else if((pre_key == SW5 || pre_key == SW4) && control_flag == CLEAR_LOG)
        {
            if(pre_key == SW5)
            clear_log();                            //READING SW5 TO CLEAR LOGS IF WE ARE IN CLEAR LOG SCREEN
            control_flag = MENU;
            reset = RESET_MENU;                     //SETTING THE RESET VALUE
        }
         press_duration = 0;                        //RESETTING THE PRESS DURATION FO THE NEXT SWITCH
        }
        switch(control_flag)                        //SWITCH TO ENTER EVERY SCREEN
        {
            case DASH_BOARD :
                display_dashboard(event,speed);     //CALLING DASHBOARD FUNCTION TO PRINT DASHBOARD ON CLCD
                break;
            case LOGIN_SCREEN:
                switch (login(&reset , pre_key))    //LOGIN FUNCTION READ AND CHECK THE PASSWORD 
                {
                    case SUCCESS :
                        control_flag = MENU;
                        TMR2ON = 0;                 //IF IT RETURN SUCCESS ENTER INTO THE MENU
                        reset = RESET_MENU;
                        break;
                    case FAILURE :
                        control_flag = DASH_BOARD;
                        TMR2ON = 0;                 //IF IT RETURN FAILURE SWITCH OFF TIMER AND ENTER INTO DASHBOARD
                        break;
                }
                break;
            case MENU:
                display_menu(pre_key , &reset);     //CALLING THE MENU FUNCTION TO DISPLAY MENU
                break;
            case VIEW_LOG :
                view_log(pre_key,&reset);           //FUNCTION TO VIEW LOGS THAT ARE IN EEPROM
                break;
            case DOWNLOAD_LOG :
                download_log();                     //FUNCTION TO SENG LOGS THROUGH UART PROTOCOL
                clear_display();
                control_flag = MENU;                //RETURN BACK TO MENU AFTER SENDING ALL DATA
                reset = RESET_MENU;
                break;
            case CHANGE_PASS :
                change_pass(pre_key , &reset);       //FUNCTION TO CHANGE PASSWORD
                break;
            case SET_TIME :
                set_time(pre_key , &reset);         //FUNCTION TO SET TIME
                break;
        }
    }
    return;
}
