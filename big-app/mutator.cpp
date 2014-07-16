#include <BPatch.h>
#include <BPatch_function.h>
#include <BPatch_point.h>
#include <dlfcn.h>
#define PROG_PATH "/usr/bin/nautilus"
int main()
{
	BPatch bpatch;
	vector<BPatch_function*> functions;
//	vector<BPatch_point *> *points;
	int mutateePid = fork();

	if(mutateePid == 0)
	{
		char *args[3] = {NULL};
		args[0] = PROG_PATH;
		args[1] = "-h";
		execvp(PROG_PATH, args);
	}
	//sleep(1);
	BPatch_process *proc = bpatch.processAttach(NULL, mutateePid);
	BPatch_image *appImage = proc->getImage();
	appImage->findFunction("main", functions);
	sleep(2);
	proc->continueExecution();

	proc->detach(true);

//	while (!proc->isTerminated()) {
//		bpatch.waitForStatusChange();
//	}

	return 0;
}
