#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>


int delay(int i)
{
	while(i--);
	reutrn 0;
}


int main(void)
{
	char val;
	int i, fp;
	fp = open("/dev/led_dev", O_RDWR);
	for (i=0; i<1000; i++)
	{
		val = 1;
		write(fp, &val, 1);

		delay(10000);

		val = 0;
		write(fp, &val, 1);
	}

	return 0;
}