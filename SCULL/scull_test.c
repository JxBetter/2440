#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
	int fp;
	int val = 1;

	fp = open("./dev/scull_dev_file", O_RDWR);
	if (fp<0)
	{
		printf("cant open");
	}

	read(fp, &val, 4);
	printf("%d\n", val);

	val = 5;

	write(fp, &val, 4);
	read(fp, &val, 4);
	printf("%d\n", val);

	return 0;
}