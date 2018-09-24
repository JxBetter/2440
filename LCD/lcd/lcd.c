#include "lcd.h"
#include "lcd_controller.h"
#define LCD_PARAMS_NUM 100

static p_lcd_params p_lcd_params_array[LCD_PARAMS_NUM];
static p_lcd_params current_lcd_params;

int register_lcd(p_lcd_params plp)
{
	int i;
	for (i=0; i<LCD_PARAMS_NUM; i++)
	{
		if (!p_lcd_params_array[i])
		{
			p_lcd_params_array[i] = plp;
			return 0;
		}
	}
	return 1;
}


int select_lcd(char *name)
{
	int i;
	for (i=0; i<LCD_PARAMS_NUM; i++)
	{
		if (p_lcd_params_array[i] && !strcmp(p_lcd_params_array[i] -> name, name))
		{
			current_lcd_params = p_lcd_params_array[i];
			return 0;
		}
	}
	return 1;
}


int lcd_enable(void)
{
	lcd_controller_enable();
	return 0;
}


int lcd_disable(void)
{
	lcd_controller_disable();
	return 0;
}


int lcd_add(void)
{
	lcd_4_3_add();
	return 0;
}


int lcd_init(void)
{
	lcd_add();
	lcd_controller_add();
	select_lcd("lcd_4_3");
	select_lcd_controller("s3c2440");
	lcd_controller_init(current_lcd_params);
	return 0;
}


int get_lcd_params(unsigned int *fb_addr, unsigned int *x, unsigned int *y, unsigned int *bpp)
{
	*fb_addr = current_lcd_params -> framebuffer_base;
	*x = current_lcd_params -> x;
	*y = current_lcd_params -> y;
	*bpp = current_lcd_params -> bpp;
	return 0;
}
