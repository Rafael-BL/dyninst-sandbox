#include <BPatch.h>
#include <BPatch_point.h>
#include <BPatch_function.h>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>

#include "../../buche/buche.h"
void closeFile(BPatch_process *proc, BPatch_thread *thread) {
	BUCHE( "Exit callback called for process...");
}

void * statusChangePollerThread(void * arg)
{
	BUCHE("Mutator: Entering poller thread");

	BPatch *bpatch = (BPatch *) arg;
	while(bpatch->waitForStatusChange())
	{
	}
	BUCHE("Mutator: Exiting poller thread");
}

int main (int argc, const char* argv[]) {
	BPatch bpatch ;
	int mutateePid = fork();

	if(mutateePid == 0)
	{
		char *args[2] = {NULL};
		args[0] = (char*) "./mutatee";
		execvp("./mutatee", args);
	}

	sleep(2);
	BUCHE("Mutator: Attaching to processs");
	BPatch_process*	proc = bpatch.processAttach(NULL, mutateePid);
	proc->stopExecution();
	BUCHE("Mutator: Continuing");
	proc->continueExecution();

	pthread_t thr;

	int ret = pthread_create(&thr, NULL, &statusChangePollerThread, &bpatch);

	sleep(10);	
	BUCHE("Mutator: Detaching")
	proc->detach(true);
	return 0;

}
