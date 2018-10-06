#include "uart.h"
#include "setup.h"
#include "nand.h"
#include "string_utils.h"

static struct tag *params;

int setup_end_tag(void);
int setup_start_tag(void);
int setup_memory_tag(void);
int setup_commandline_tag(char *cmd);


int main(void)
{
	void (*thekernel)(int zero, int arch, unsigned int params);
	uart0_init();
	/* 从nand把内核拷贝到sdram */
	transmits("copy to sram\n\r");
	read_nandflash(0x60040, (unsigned char *)0x30008000, 2*1024*1024);

	/* 初始化参数 */
	transmits("init kernel params\n\r");
	setup_start_tag();
	setup_memory_tag();
	setup_commandline_tag("noinitrd root=/dev/mtdblock3 init=/linuxrc console=ttySAC0,9600");
	setup_end_tag();

	/* 从内核的地址启动内核 */
	transmits("start kernel...\n\r");
	thekernel = (void (*)(int, int, unsigned int))0x30008000;
	thekernel(0, 362, 0x30000100);

	transmits("something wrong!\n\r");
	return -1;
}

int setup_start_tag(void)
{
	params = (struct tag *)0x30000100;
	params -> hdr.tag = ATAG_CORE;
	params -> hdr.size = tag_size(tag_core);
	params -> u.core.flags = 0;
	params -> u.core.pagesize = 0;
	params -> u.core.rootdev = 0;

	params = tag_next(params);
	return 0;
}


int setup_commandline_tag(char *cmd)
{
	params -> hdr.tag = ATAG_CMDLINE;
	params -> hdr.size = ((sizeof(struct tag_header)) + strlen(cmd) + 1 + 3) >> 2;

	strcpy(params -> u.cmdline.cmdline, cmd);

	params = tag_next(params);
	return 0;
}


int setup_memory_tag(void)
{
	params -> hdr.tag = ATAG_MEM;
	params -> hdr.size = tag_size(tag_mem32);

	params -> u.mem.start = 0x30000000;
	params -> u.mem.size = 0x4000000;

	params = tag_next(params);
	return 0;
}


int setup_end_tag(void)
{
	params -> hdr.tag = ATAG_NONE;
	params -> hdr.size = 0;

	return 0;

}

