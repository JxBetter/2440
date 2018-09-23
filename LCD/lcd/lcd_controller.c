#include "lcd_controller.h"
#include "lcd.h"
#define LCD_CONTROLLER_NUM 100

static p_lcd_controller lcd_controller_array[LCD_CONTROLLER_NUM];
static p_lcd_controller current_lcd_controller;


int register_lcd_controller(p_lcd_controller plc)
{
	int i;
	for (i=0; i<LCD_CONTROLLER_NUM; i++)
	{
		if (!lcd_controller_array[i])
		{
			lcd_controller_array[i] = plc;
			return 0;
		}
	}
	return 1;
}


int select_lcd_controller(char *name)
{
	int i;
	for (i=0; i<LCD_CONTROLLER_NUM; i++)
	{
		if (lcd_controller_array[i] && !strcmp(lcd_controller_array[i] -> name, name))
		{
			current_lcd_controller = lcd_controller_array[i];
			return 0;
		}
	}
	return 1;
}


int lcd_controller_init(p_lcd_params plp)
{
	if (current_lcd_controller)
	{
		current_lcd_controller -> init(plp);		
		return 0;
	}
	return 1;
}


int lcd_controller_enable(void)
{
	if (current_lcd_controller)
	{
		current_lcd_controller -> enable();
		return 0;
	}
	return 1;
}


int lcd_controller_disable(void)
{
	if (current_lcd_controller)
	{
		current_lcd_controller -> disable();
		return 0;
	}
	return 1;
}


int lcd_controller_add(void)
{
	s3c2440_lcd_controller_add();
	return 0;
}