#include "lcd.h"
#define NORMAL 0
#define INVERT 1


lcd_params lcd_4_3_params = 
{
	.name = "lcd_4_3",
	.lcd_pins_config = 
	{
		.de = NORMAL,
		.pwren = NORMAL,
		.vclk = NORMAL,
		.rgb = NORMAL,
		.hsync = INVERT,
		.vsync = INVERT,
	},
	.time_sequence_config = 
	{
		/* 垂直方向 */
		.tvp = 10, /* vsync脉冲宽度 */
		.tvb = 2, /* 上黑框 */
		.tvf = 2, /* 下黑框 */

		/* 水平方向 */
		.thp = 41, /* hsync脉冲宽度 */
		.thb = 2, /* 左黑框 */
		.thf = 2, /* 右黑框 */

		.vclk = 9,
	},
	.x = 480,
	.y = 272,
	.bpp = 16,
	.framebuffer_base = 0x32000000,
};


int lcd_4_3_add(void)
{
	register_lcd(&lcd_4_3_params);
}