#include "s3c2440_soc.h"
#include "uart.h"

int timer_init(void)
{
	/* 设置定时器时钟 */
	/* Timer clk = PCLK / prescaler + 1 / divider */
	TCFG0 = 99;
	TCFG1 &= ~0x0f;
	TCFG1 |= 3;

	/* 设置初值，一秒中断一次*/
	TCNTB0 = 31250*2;

	/* 加载初值 */
	TCON |= 1<<1; 

	/* 设置为自动加载并启动 */
	TCON &= ~(1<<1);
	TCON |= ((1<<0) | (1<<3));

	/* 设置中断 */


}

