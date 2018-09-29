#include "my_printf.h"
#include "s3c2440_soc.h"
#include "string_utils.h"


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


int nand_write_data(unsigned char data)
{
	NFDATA = data;
	return 0;
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


int erase_nand(unsigned int addr, unsigned int len)
{
	int page;
	if ((addr & 0x1ffff) || (len & 0x1ffff))
		return -1;
	nand_select_chip();

	while(1)
	{
		page = addr / 2048;
		nand_command(0x60);
		nand_addr(page & 0xff);
		nand_addr((page >> 8) & 0xff);
		nand_addr((page >> 16) & 0xff);
		nand_command(0xD0);
		wait();

		len -= (1024*128);
		if (len <= 0)
			break;

		addr += (1024*128);
	}
	nand_deselect_chip();
	return 0;
}


int write_nand(unsigned int addr, unsigned char *buff, unsigned int len)
{
	nand_select_chip();
	int i, page, col;
	i = 0;
	page = addr / 2048;
	col = addr % 2048;

	while(1)
	{
		nand_command(0x80);
		nand_addr(col & 0xff);
		nand_addr((col >> 8) & 0xff);
		nand_addr(page & 0xff);
		nand_addr((page >> 8) & 0xff);
		nand_addr((page >> 16) & 0xff);

		for (;(col<2048) && (i<len);col++)
		{
			nand_write_data(buff[i++]);
		}
		nand_command(0x10);
		wait();
		col = 0;
		if (i == len)
			break;
		page++;
	}
}


int do_erase_nandflash(void)
{
	unsigned int addr;
	
	/* »ñµÃµØÖ· */
	printf("Enter the address of sector to erase: ");
	addr = get_uint();

	printf("erasing ...\n\r");
	erase_nand(addr, 128*1024);
}


void do_read_nandflash(void)
{
	unsigned int addr;
	volatile unsigned char *p;
	int i, j;
	unsigned char c;
	unsigned char str[16];
	unsigned char buf[64];
	
	/* »ñµÃµØÖ· */
	printf("Enter the address to read: ");
	addr = get_uint();

	read_nandflash(addr, buf, 64);
	p = (volatile unsigned char *)buf;

	printf("Data : \n\r");

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 16; j++)
		{
			c = *p++;
			str[j] = c;
			printf("%02x ", c);
		}

		printf("   ; ");

		for (j = 0; j < 16; j++)
		{
			if (str[j] < 0x20 || str[j] > 0x7e)
				transmit('.');
			else
				transmit(str[j]);
		}
		printf("\n\r");
	}
}


void do_write_nandflash(void)
{
	unsigned int addr;
	unsigned char str[100];
	int i, j;
	unsigned int val;
	
	printf("Enter the address of sector to write: ");
	addr = get_uint();

	printf("Enter the string to write: ");
	gets(str);

	printf("writing ...\n\r");
	write_nand(addr, str, strlen(str)+1);

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
				do_erase_nandflash();
				break;
			case 'r':
				do_read_nandflash();
				break;
			case 'w':
				do_write_nandflash();
				break;
			default :
				break;
		}
	}
}
