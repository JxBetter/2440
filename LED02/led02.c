#include <unistd.h>

#define GPFCON (*(volatile unsigned int *)(0x56000050))
#define GPFDAT (*(volatile unsigned int *)(0x56000054))

int main(void)
{
	int flag = 0;
	int j;
	//unsigned int *pGPFCON = 0x56000050;
	//unsigned int *pGPFDAT = 0x56000054;
	//*pGPFDAT = 0;
	//*pGPFCON = 0x1400;
	GPFCON &= ~( (3<<8) | (3<<10) | (3<<12) );
	GPFCON |= ( (1<<8) | (1<<10) | (1<<12) );

	while(1)
	{
		if(flag == 0)
		{
			GPFDAT = 0x10;
			flag += 1;
		}
		else if(flag == 1)
		{
			GPFDAT = 0x20;
			flag += 1;
		}
		else if(flag == 2)
		{
			GPFDAT = 0x40;
			flag = 0;
		}
		for(j=0;j<5000;j++);
	}

}