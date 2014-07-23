
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

void  foo(int bar)
{
    	printf("%d",bar);
	return;
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	foo(rand());
	return 0;
}
