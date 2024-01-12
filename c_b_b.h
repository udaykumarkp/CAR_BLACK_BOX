#ifndef C_B_B_H
#define	C_B_B_H

//FUNCTION TO DISPLAY DASHBOARD
void display_dashboard(unsigned char *event,unsigned char speed);
//FUNCTION TO STORE LOG INTO  ROM
void log_event(unsigned char *event,unsigned char speed);
//FUCNTION TO DISPLAY LOGIN SCREEN
char login(unsigned char *reset, unsigned char key);
//FUNCTION TO PRINT DEFAULT LINE IN LOGIN SCREEN
void login_screen(void);
//FUNCTION TO DISPLAY MENU
char display_menu(unsigned char key , unsigned char *reset);
//FUNTION FOR VIEW LOG SCREEN
void view_log(unsigned char key ,unsigned char* reset);
//FUNCTION TO FETCH DATA FROM EEPROM
void fetch_data_from_memory(unsigned char i);
//FUNCTION TO CLEAR ALL LOGS OF ROM
void clear_log(void);
//FUNCTION TO GET GET DATA FROM UART
void download_log(void);
//FUNTION CHANGE PASSWORD
void change_pass(unsigned char key ,unsigned char * reset);
//FUNCTION TO EDIT TIME
void set_time(unsigned char key , unsigned char * reset);
//FUNCTION TO SAVE TIME AFTER EDITING
void save_time(void);

#endif	/* C_B_B_H */

