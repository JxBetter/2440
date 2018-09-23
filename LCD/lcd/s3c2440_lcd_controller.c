#include "lcd_controller.h"
#include "s3c2440_soc.h"
#define HCLK 100


int s3c2440_lcd_controller_init(p_lcd_params p);
int s3c2440_lcd_controller_enable(void);
int s3c2440_lcd_controller_disable(void);


struct lcd_controller s3c2440_lcd_controller = 
{
	.name = "s3c2440",
	.init = s3c2440_lcd_controller_init,
	.enable = s3c2440_lcd_controller_enable,
	.disable = s3c2440_lcd_controller_disable,
};


int s3c2440_pins_init(void)
{
	/* 初始化引脚 : 背光引脚 */
	GPBCON &= ~0x3;
	GPBCON |= 0x01;

	/* LCD专用引脚 */
	GPCCON = 0xaaaaaaaa;
	GPDCON = 0xaaaaaaaa;

	/* PWREN */
	GPGCON |= (3<<8);

	return 0;
}

/* 根据LCD参数初始化控制器 */
int s3c2440_lcd_controller_init(p_lcd_params p)
{
	unsigned int fb_addr;
	s3c2440_pins_init();
	/*
	 * LCDCON1:
	 *
	 * [17:8] vclk, vclk = HCLK / ((CLKVAL+1) * 2)
	 * 				CLKVAL = HCLK / vclk / 2 - 1 
	 *
	 * [6:5] 设置为11,为tft屏幕
	 *
	 * [4:1] 设置bpp模式
	 *
	 * [0] lcd video使能位
	 */
	int clkval = (double)HCLK / p->time_sequence_config.vclk / 2 - 1 + 0.5;
	int bppmode = p->bpp == 8  ? 0xb :\
			      p->bpp == 16 ? 0xc :\
			  				     0xd;
	LCDCON1 = (clkval << 8) | (3 << 5) | (bppmode << 1);

	/*
	 * LCDCON2:
	 *
	 * [31:24] : VBPD    = tvb - 1
	 * [23:14] : LINEVAL = line - 1
	 * [13:6]  : VFPD    = tvf - 1
	 * [5:0]   : VSPW    = tvp - 1
	 */

	LCDCON2 = ((p->time_sequence_config.tvb - 1) << 24) | \
	((p->y - 1) << 14) | \
	((p->time_sequence_config.tvf - 1) << 6) | \
	((p->time_sequence_config.tvp - 1) << 0);
	/* 
	 * LCDCON3:
	 *
	 * [25:19] : HBPD	 = thb - 1
	 * [18:8]  : HOZVAL  = 列 - 1
	 * [7:0]   : HFPD	 = thf - 1
	 */

	LCDCON3 = ((p->time_sequence_config.thb - 1) << 19) | \
				((p->x - 1) << 8) | \
				((p->time_sequence_config.thf - 1) << 0);

	/* 
	 * LCDCON4:
	 *
	 * [7:0]   : HSPW	 = thp - 1
	 */

	LCDCON4 = ((p->time_sequence_config.thp - 1) << 0);

	/*
	 * LCDCON5:
	 *
	 * 用来设置引脚极性, 设置16bpp, 设置内存中象素存放的格式
     * [12] : BPP24BL
	 * [11] : FRM565, 1-565
	 * [10] : INVVCLK, 0 = The video data is fetched at VCLK falling edge
	 * [9]  : HSYNC是否反转
	 * [8]  : VSYNC是否反转
	 * [7]  : INVVD, rgb是否反转
	 * [6]  : INVVDEN
	 * [5]  : INVPWREN
	 * [4]  : INVLEND
	 * [3]  : PWREN, LCD_PWREN output signal enable/disable
	 * [2]  : ENLEND
	 * [1]  : BSWP
	 * [0]  : HWSWP
	 */

	int piexl_mode = p->bpp == 24 ? 0 : \
						p->bpp == 16 ? 1 : \
						(1<<1);
	LCDCON5 = ((p->lcd_pins_config.vclk) << 10) | \
				((p->lcd_pins_config.rgb) << 7) | \
				((p->lcd_pins_config.hsync) << 9) | \
				((p->lcd_pins_config.vsync) << 8) | \
				((p->lcd_pins_config.de) << 6) | \
				((p->lcd_pins_config.pwren) << 5) | \
				(1<<11) | piexl_mode;

	/*
	 * LCDSADDR1:
	 * 设置显存地址
	 * [29:21] : LCDBANK, A[30:22] of fb
	 * [20:0]  : LCDBASEU, A[21:1] of fb
	 */
	fb_addr = p->framebuffer_base & ~(1<<31);
	fb_addr >>= 1;
	LCDSADDR1 = fb_addr;

	/* 
	 * LCDSADDR2:
	 *
	 * [20:0] : LCDBASEL, A[21:1] of end addr
	 */

	fb_addr = p->framebuffer_base + p->x * p->y *p->bpp / 8;
	fb_addr >>= 1;
	fb_addr &= (0x1fffff);
	LCDSADDR2 = fb_addr;

	return 0;
}


int s3c2440_lcd_controller_enable(void)
{
	/* 背光引脚 : GPB0 */
	GPBDAT |= (1<<0);
	
	/* pwren    : 给LCD提供AVDD  */
	LCDCON5 |= (1<<3);
	
	/* LCDCON1'BIT 0 : 设置LCD控制器是否输出信号 */
	LCDCON1 |= (1<<0);

	return 0;
}


int s3c2440_lcd_controller_disable(void)
{
	/* 背光引脚 : GPB0 */
	GPBDAT &= ~(1<<0);

	/* pwren	: 给LCD提供AVDD  */
	LCDCON5 &= ~(1<<3);

	/* LCDCON1'BIT 0 : 设置LCD控制器是否输出信号 */
	LCDCON1 &= ~(1<<0);

	return 0;
}


int s3c2440_lcd_controller_add(void)
{
	register_lcd_controller(&s3c2440_lcd_controller);
	return 0;
}