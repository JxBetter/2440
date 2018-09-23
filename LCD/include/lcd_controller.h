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

int register_lcd_controller(p_lcd_controller plc);
int select_lcd_controller(char *name);
int lcd_controller_init(p_lcd_params plp);
int lcd_controller_enable(void);
int lcd_controller_disable(void);
int lcd_controller_add(void);
int s3c2440_lcd_controller_add(void);

#endif