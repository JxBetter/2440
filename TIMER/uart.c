#define PCLK 50000000
#define BARD_RATE 115200
#define UBRDIV  (int)( PCLK / ( BARD_RATE x 16) ) –1
#define UBRDIV0 (*(volatile unsigned int *)(0x50000028))
#define GPHCON (*(volatile unsigned int *)(0x56000070))
#define ULCON0 (*(volatile unsigned int *)(0x50000000))
#define UCON0 (*(volatile unsigned int *)(0x50000004))
#define UFCON (*(volatile unsigned int *)(0x50004008))
#define UMCON (*(volatile unsigned int *)(0x5000400C))
#define UTRSTAT0 (*(volatile unsigned int *)(0x50000010))
#define  UTXH0 (*(volatile unsigned char *)(0x50000020))
#define URXH0 (*(volatile unsigned char *)(0x50000024))
#define GPHUP (*(volatile unsigned int *)(0x56000078))

int uart1_init(void)
{
	/* 配置GPH4,GPH5管脚为串口模式 */
	GPHCON &= ~(0xF<<4);
	GPHCON |= (0xA<<4);
	GPHUP &= ~(0x3<<2);

	/* 设置波特率 */
	UBRDIV0 = 324;

	/* 设置传输格式 */
	ULCON0 &= ~(0x3<<0);
	ULCON0 |= (0x3<<0);

	/* 设置时钟源和中断方式 */
	UCON0 &= ~(0xF<<0);
	UCON0 |= (0x5<<0);

	/* 关闭流控 */
	UMCON = 0x00;

	/* 不使用FIFO */
	UFCON = 0x00;

	return 0;
}


int transmit(int c)
{
	/* UTRSTAT0 */
	/* UTXH0 */

	while (!(UTRSTAT0 & (1<<2)));
	UTXH0 = (unsigned char)c;
	
}


int receive(void)
{
	while (!(UTRSTAT0 & (1<<0)));
	return URXH0;
}

int transmits(const char *s)
{
	while (*s)
	{
		transmit(*s);
		s++;
	}
}


int printHex(unsigned int val)
{
	int i;
	unsigned char arr[8];

	/* 先取出每一位的值 */
	for (i = 0; i < 8; i++)
	{
		arr[i] = val & 0xf;
		val >>= 4;   /* arr[0] = 2, arr[1] = 1, arr[2] = 0xF */
	}

	/* 打印 */
	transmits("0x");
	for (i = 7; i >=0; i--)
	{
		if (arr[i] >= 0 && arr[i] <= 9)
			transmit(arr[i] + '0');
		else if(arr[i] >= 0xA && arr[i] <= 0xF)
			transmit(arr[i] - 0xA + 'A');
	}
	return 0;
}


int print1(void)
{
	transmits("123");
}