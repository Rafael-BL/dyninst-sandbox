#include <BPatch.h>
#include <BPatch_point.h>
#include <BPatch_function.h>
#include <stdio.h>
#include <unistd.h>

#include "../../buche/buche.h"

int main (int argc, const char* argv[]) {
	BPatch bpatch;
	//BPatch_process *handle = bpatch.processCreate("./mutatee", NULL);

	int mutateePid = fork();

    	if(mutateePid == 0)
    	{
		char *args[4] = {NULL};
	//	args[0] = (char *) "env";
	//	args[1] = (char *) "LD_PRELOAD=/usr/local/lib/liblttng-ust.so";
	//	args[2] = (char *) "./mutatee";
		execvp("./mutatee", args);
    	}

    	sleep(1);
    	BPatch_process *handle = bpatch.processAttach(NULL, mutateePid);

	BUCHE("Beginning of mutator");
	vector<BPatch_variableExpr*> vec;	
	int tot = 0;
	int inc = 1000000;
	for(int i = 0;i < 1000;i++)
	{
		BPatch_variableExpr *expr = handle->malloc(inc);
		vec.push_back(expr);
		tot+=inc;
		printf("total=%d bytes, inc=%d bytes\n", tot, inc);
	}
	printf("Mutator: continueExecution\n");
	handle->continueExecution();

	while (!handle->isTerminated())
		bpatch.waitForStatusChange();
	return 0;
}
