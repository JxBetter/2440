#ifndef _LCD_
#define _LCD_


enum polarity
{
	NORMAL = 0,
	INVERT = 1,
};


/* 极性结构体 */
typedef struct lcd_pins
{
	int de;
	int pwren;
	int vclk;
	int rgb;
	int hsync;
	int vsync;
}lcd_pins, *p_lcd_pins;


/* 时序 */
typedef struct time_sequence
{
	/* 垂直方向 */
	int tvp; /* vsync脉冲宽度 */
	int tvb; /* 上黑框 */
	int tvf; /* 下黑框 */

	/* 水平方向 */
	int thp; /* hsync脉冲宽度 */
	int thb; /* 左黑框 */
	int thf; /* 右黑框 */

	int vclk;
}time_sequence, *p_time_sequence;


typedef struct lcd_params
{
	char *name;
	/* 引脚极性 */
	lcd_pins lcd_pins_config;

	/* 时序 */
	time_sequence time_sequence_config;

	/* 分辨率和bpp*/
	int x;
	int y;
	int bpp;

	/* 显存地址 */
	unsigned int framebuffer_base;
	
}lcd_params, *p_lcd_params;

int register_lcd(p_lcd_params plp);
int select_lcd(char *name);
int lcd_enable(void);
int lcd_disable(void);
int lcd_add(void);
int lcd_init(void);
int lcd_4_3_add(void);

#endif