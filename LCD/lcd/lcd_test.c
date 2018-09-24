#include "lcd.h"
#include "draw.h"

int lcd_test(void)
{
	int i, j;
	unsigned int fb_addr;
	unsigned int x;
	unsigned int y;
	unsigned int bpp;
	unsigned short *p;

	lcd_init();
	lcd_enable();
	get_lcd_params(&fb_addr, &x, &y, &bpp);
	p = (unsigned short *)(fb_addr);

	/* 565: RGB,两个字节，所以用unsigned short */
	if (16 == bpp)
	{
		for (i=0; i<x; i++)
		{
			for (j=0; j<y; j++)
			{
				*p++ = 0xd2f5;
			}
		}
	}

	draw_get_params();
	draw_string(20, 20, "gujinxin\n\rgaozhuorong!!", 0x228B22);
	draw_line(140, 86, 340, 86, 0x228B22);
	draw_line(340, 86, 340, 186, 0x228B22);
	draw_line(340, 186, 140, 186, 0x228B22);
	draw_line(140, 186, 140, 86, 0x228B22);
	draw_line(140, 86, 340, 186, 0x228B22);
	draw_line(340, 86, 140, 186, 0x228B22);
	draw_circle(240, 136, 50, 0x228B22);


}