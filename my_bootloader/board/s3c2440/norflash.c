#include "uart.h"
#include "string_utils.h"
#include "my_printf.h"

#define NORFLASH_BASE 0

int norflash_return(unsigned int offset_addr);
int erase_norflash(void);
int read_norflash(void);
int write_norflash(void);
int unlock_norflash(void);
int wait_ready(unsigned int addr);

int norflash_test(void)
{
	/* 测试函数 */
	char enter;
	while(1)
	{
		printf("[s] Scan norflash\n\r");
		printf("[e] Erase norflash\n\r");
		printf("[r] Read norflash\n\r");
		printf("[1] Simple Read norflash\n\r");
		printf("[w] Write norflash\n\r");
		printf("[q] quit\n\r");
		printf("Enter your choice: ");

		enter = receive();
		transmit(enter);
		printf("\n\r");

		switch (enter)
		{
			case 's':
				scan_norflash();
				break;
			case 'e':
				erase_norflash();
				break;
			case 'r':
				read_norflash();
				break;
			case '1':
				simple_read();
				break;
			case 'w':
				write_norflash();
				break;
			default :
				break;
		}
	}
}


int _write_data_to_addr(unsigned int base, unsigned int offset, unsigned short data)
{
	/* 根据norflash所接片选不同，基地址也不同 */
	/* 将norflash地址转换为cpu地址，左移一位 */
	volatile unsigned short *addr = (volatile unsigned short *)(base + (offset << 1));
	*addr = data;
	return 0;
}


unsigned int _read_data_from_addr(unsigned int base, unsigned int offset)
{
	volatile unsigned short *addr = (volatile unsigned short *)(base + (offset << 1));
	return *addr;
}


int norflash_cmd(unsigned int offset_addr, unsigned short cmd_data)
{
	_write_data_to_addr(NORFLASH_BASE, offset_addr, cmd_data);
	return 0;
}


int norflash_return(unsigned int offset_addr)
{
	return _read_data_from_addr(NORFLASH_BASE, offset_addr);
}


int unlock_norflash(void)
{
	norflash_cmd(0x555, 0xaa);
	norflash_cmd(0x2aa, 0x55);
	return 0;
}


int wait_ready(unsigned int addr)
{
	unsigned int first, second;
	first = norflash_return(addr);
	second = norflash_return(addr);
	while ((first & (1<<6)) != (second & (1<<6)))
	{
		second = first;
		first = norflash_return(addr);
	}
	return 0;
}


int scan_norflash(void)
{
	/* 打印厂家ID，设备ID，容量， 各个扇区其起始地址 
	 * 进入norflash的CFI模式
	 */
	int i, j, cnt;
	char str[4];
	unsigned int norflash_size;
	unsigned int manu_id, device_id;
	unsigned int num_of_region, num_of_block, size_of_block, region_cmd_base, block_addr;

	/* 厂家ID，设备ID */
	unlock_norflash();
	norflash_cmd(0x555, 0x90);
	manu_id = norflash_return(0);
	device_id = norflash_return(1);
	norflash_cmd(0, 0xf0);
	printf("manu_id = 0x%x, device_id = 0x%x\n\r", manu_id, device_id);

	norflash_cmd(0x55, 0x98); //进入CFI模式
	str[0] = norflash_return(0x10);
	str[1] = norflash_return(0x11);
	str[2] = norflash_return(0x12);
	str[3] = '\0';
	printf("str = %s\n\r", str);

	/* 容量 */
	norflash_size = 1 << norflash_return(0x27);
	printf("norflash size = %dM\n\r", norflash_size/(1024*1024));

	/* norflash中每一个扇区的起始地址，相对于cpu的地址，这个地址>>1才是Norflash的地址 */
	cnt = 0;
	block_addr = 0;
	region_cmd_base = 0x2d;

	num_of_region = norflash_return(0x2c);

	for (i=0; i<num_of_region; i++)
	{
		num_of_block = norflash_return(region_cmd_base) + (norflash_return(region_cmd_base+1) << 8) + 1;
		size_of_block = (norflash_return(region_cmd_base+2) + (norflash_return(region_cmd_base+3) << 8)) * 256;
		region_cmd_base += 4;
		for (j=0; j<num_of_block; j++)
		{
			cnt++;
			printHex(block_addr);
			printf(" ");
			block_addr += size_of_block;
			if (cnt % 5 == 0)
			{
				printf("\n\r");
			}

		}
	}
	/* 退出CFI */
	norflash_cmd(0, 0xf0);
}



int erase_norflash(void)
{
	unsigned int addr;

	printf("Enter address of sector to earse: ");
	addr = get_uint();

	unlock_norflash();
	norflash_cmd(0x555, 0x80);
	unlock_norflash();
	norflash_cmd(addr>>1, 0x30);
	wait_ready(addr>>1);

	return 0;
}


int read_norflash(void)
{
	/* 读的时候按照cpu的角度去读，地址是cpu角度的，不是norflash角度的 */
	int i, j;
	unsigned char c;
	unsigned int addr;
	unsigned char str[64];
	volatile unsigned char *p;

	printf("Enter address to read: ");
	addr = get_uint();
	p = (volatile unsigned char *)addr;

	for (i=0; i<4; i++)
	{
		/* 先打印16个数值 */
		for (j=0; j<16; j++)
		{
			str[j] = *p++;
			printf("%02x ", str[j]);
		}
		printf("    ; ");

		/* 后打印16个对应字符 */
		for (j=0; j<16; j++)
		{
			if (str[j] < 0x20 || str[j] > 0x7e)
				transmit('.');
			else
				transmit(str[j]);
		}
		printf("\n\r");
	}
	return 0;
}


int write_norflash(void)
{
	/* 写的时候地址要从cpu的角度来写，否则写入的地址是x，读的时候需要到2x的地方去读 */
	/* 读写时，地址角度要相同，因为读是从cpu角度去读的，所以写的时候地址也要按照cpu的角度去写 */
	int i = 0;
	unsigned int addr, val;
	unsigned char str[100];

	printf("Enter address of sector to write: ");
	addr = get_uint();

	printf("Enter data to write: ");
	gets(str);

	printf("Writing ...\n\r");

	while (str[i] && str[i+1])
	{
		val = str[i] + (str[i+1] << 8);
		unlock_norflash();
		norflash_cmd(0x555, 0xa0);
		norflash_cmd(addr>>1, val);
		wait_ready(addr>>1);
		addr += 2;
		i += 2;
	}
	val = str[i];
	unlock_norflash();
	norflash_cmd(0x555, 0xa0);
	norflash_cmd(addr>>1, val);
	wait_ready(addr>>1);

	return 0;
}


int simple_read(void)
{
	unsigned int addr;
	volatile unsigned char *p;

	printf("Enter addr to read: ");
	addr = get_uint();
	p = (volatile unsigned char *)addr;

	printf("data is %02x\n\r", *p); 
}

