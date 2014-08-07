#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "../../buche/buche.h"
static void *thread1(void * arg)
{
	int i = 10;
	while(i > 0){
	    	BUCHE("Mutatee: %d. Thread1",i);
	    	sleep(1);
	    	i--;
	}
}

int main(int argc, char **argv)
{
	printf("Mutatee: Starting\n");
	pthread_t thr;
	void *res;
	int ret = pthread_create(&thr, NULL, &thread1,NULL);
	sleep(10);
	BUCHE("Mutatee: Trying to cancel");
	ret = pthread_cancel(thr);
	ret = pthread_join(thr, &res);
	sleep(1);
	BUCHE("Mutatee: Exiting");
	return 0;
}
