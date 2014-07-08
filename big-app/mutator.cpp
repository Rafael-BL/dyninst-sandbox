#include <BPatch.h>
int main()
{
	BPatch bpatch;
	int mutateePid = fork();

	if(mutateePid == 0)
	{
		char *args[3] = {NULL};
		args[0] = "/usr/lib/firefox/firefox";
		args[1] = "-h";
		execvp("/usr/lib/firefox/firefox", args);
	}
	usleep(500);
	BPatch_process *proc = bpatch.processAttach(NULL, mutateePid);
	proc->continueExecution();

	while (!proc->isTerminated()) {
		bpatch.waitForStatusChange();
	}
	proc->detach(true);

	return 0;
}
