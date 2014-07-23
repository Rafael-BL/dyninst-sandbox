#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "../../buche/buche.h"
void foo(void *str)
{
    	printf("mutatee: %s\n",(char*)str);
	return;
}

int main(int argc, char **argv)
{
	sleep(2);
	BUCHE("Beginning of main");
	return 0;
}
