#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void foo(void)
{
    	printf("in foo\n");
	return;
}

int main(int argc, char **argv)
{
	printf("Mutatee Starting\n");
	sleep(1);
	int i;
	for(i = 0; i < 5; ++i)
	{
		foo();
		sleep(1);
	}
	sleep(1);
	return 0;
}
