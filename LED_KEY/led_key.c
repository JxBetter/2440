#define GPFCON (*(volatile unsigned int *)(0x56000050))
#define GPFDAT (*(volatile unsigned int *)(0x56000054))

int main(void)
{
	int data;
	/* 初始化led（GPF5），引脚为输出模式 */
	GPFCON &= ~(3<<10);
	GPFCON |= (1<<10);

	/* 初始化EINT2（GPF2），引脚为出入模式 */
	GPFCON &= ~(3<<4);

	while(1)
	{
		data = GPFDAT;
		if(data & (1<<2))
		{
			/* 默认EINT2为高电平，寄存器为1，按键没有按下 */
			GPFDAT |= (1<<5);
		}
		else
		{
			/* 按键按下 */
			GPFDAT &= ~(1<<5);
		}
	}
}