#include "s3c2440_soc.h"


int nand_select_chip(void)
{
	NFCONT &= ~(1<<1);
	return 0;
}


int nand_deselect_chip(void)
{
	NFCONT &= ~(1<<1);
	NFCONT |= (1<<1);
	return 0;
}


int nand_command(unsigned char cmd)
{
	volatile int i;
	NFCMD = cmd;
	for(i=0; i<10; i++);
	return 0;
}


int nand_addr(unsigned char addr)
{
	volatile int i;
	NFADDR = addr;
	for(i=0; i<10; i++);
	return 0;
}


unsigned char nand_data(void)
{
	return NFDATA;
}


int nandflash_init(void)
{
	/* 初始化时序 */
	NFCONF = (0<<12) | (1<<8) | (0<<4);

	/* 使能控制器 */
	NFCONT = ((1<<0) | (1<<1) | (1<<4));
}


int wait(void)
{
	while (!(NFSTAT & 1));
}


int nand_read_id(void)
{	
	unsigned int i, data[5];
	/* 片选使能 */
	nand_select_chip();

	/* 发出读id命令 */
	nand_command(0x90);

	/* 发出地址信号 */
	nand_addr(0x00);

	for (i=0; i<5; i++)
	{
		data[i] = nand_data();
		printf("0x%x\n\r", data[i]);
	}
	printf("page size = %dkb, block size = %dkb\n\r", (1 << (data[3] & 0x03)), (64 << ((data[3] >> 4) & 0x03)));
	nand_deselect_chip();
	return 0;
}


int read_nandflash(unsigned int addr, unsigned char *buff, unsigned int len)
{
	unsigned int i, page, col;
	i = 0;
	page = addr / 2048;
	col = addr % 2048;
	nand_select_chip();

	while (i<len)
	{
		/* 发出0x00命令 */
		nand_command(0x00);

		/* 发出地址信号 */
		nand_addr(col & 0xff);
		nand_addr((col >> 8) & 0xff);
		nand_addr(page & 0xff);
		nand_addr((page >> 8) & 0xff);
		nand_addr((page >> 16) & 0xff);

		/* 发出0x30命令 */
		nand_command(0x30);

		/* 等待数据稳定 */
		wait();

		/* 读取数据 */
		for (;(col<2048) && (i<len); col++)
		{
			buff[i++] = nand_data();
		}
		if (i==len)
			break;
		col = 0;
		page ++;
	}
	nand_deselect_chip();
	return 0;
}


int nandflash_test(void)
{
	/* 测试函数 */
	char enter;
	unsigned char buff[2048];
	while(1)
	{
		printf("[s] Scan nandflash\n\r");
		printf("[e] Erase nandflash\n\r");
		printf("[r] Read nandflash\n\r");
		printf("[w] Write nandflash\n\r");
		printf("[q] quit\n\r");
		printf("Enter your choice: ");

		enter = receive();
		transmit(enter);
		printf("\n\r");

		switch (enter)
		{
			case 's':
				nand_read_id();
				break;
			case 'e':
				//erase_norflash();
				break;
			case 'r':
				read_nandflash(0x00, buff, 8);
				break;
			case 'w':
				//write_norflash();
				break;
			default :
				break;
		}
	}
}
