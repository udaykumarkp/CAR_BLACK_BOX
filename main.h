#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>
#include <string.h>
#include "adc.h"
#include "digital_keypad.h"
#include "clcd.h"
#include "ds1307.h"
#include "i2c.h"
#include "c_b_b.h"
#include "EEprom.h"
#include "timers.h"
#include "uart.h"

#define DASH_BOARD      0x01
#define LOGIN_SCREEN    0x02
#define MENU            0x03
#define VIEW_LOG        0x05
#define DOWNLOAD_LOG    0x06
#define CHANGE_PASS     0x07
#define SET_TIME        0x08
#define CLEAR_LOG       0x09
#define RESET_PASS      0x0A
#define RESET_NOTHING   0x0B
#define RESET_MENU      0x0C
#define RESET_VIEW      0x0D
#define RESET_CHANGE    0x0E
#define RESET_TIME      0x0F
#define SUCCESS         0x11
#define FAILURE         0x00

#endif	/* MAIN_H */

