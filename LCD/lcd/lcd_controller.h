#ifndef _LCD_CONTROLLER_
#define _LCD_CONTROLLER_

#include "lcd.h"

typedef struct lcd_controller
{
	char *name;
	int(*init)(p_lcd_params p);
	int(*enable)(void);
	int(*disable)(void);
}lcd_controller, *p_lcd_controller;

#endif