#include <stdio.h>
#include <unistd.h>

void print(int a, short b, char *c, char d)
{
	printf("%d, %d, %s, %c\n",a ,b ,c ,d);
}


int main()
{
	int i;
	setbuf(stdout, NULL);
	printf("looping");
	for(i = 0; i < 3; i++)
	{
		printf(".");
		sleep(2);
	}
	printf(" Done!\n");
	print(1337, 86, "Hello World", 'c');
	for(;;){}
	return 0;
}
