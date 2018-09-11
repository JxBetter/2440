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
	NFCMD = cmd;
	return 0;
}


int nand_addr(unsigned char addr)
{
	NFADDR = addr;
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
	printf("page size = %d, block size = %d\n\r", (1 << (data[3] & 0x03)), (64 << ((data[3] >> 4) & 0x03)));
	nand_deselect_chip();

}
