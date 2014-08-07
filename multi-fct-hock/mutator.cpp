#include <BPatch.h>
#include <BPatch_point.h>
#include <BPatch_function.h>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "argtype.h"

#define DYNSO_PATH "./dyn.so"
int main (int argc, const char* argv[]) {
	BPatch bpatch;
	//BPatch_process *proc = bpatch.processCreate(argv[2], argv + 2);
	int mutateePid = fork();

	if(mutateePid == 0)
	{
		char *args[3] = {NULL};
		args[0] = (char*) "../suspect";
		args[1] = (char*)"1";
		execvp("../suspect", args);
	}

	sleep(1);
	BPatch_process *proc = bpatch.processAttach(NULL, mutateePid);

	if (proc == NULL)
	{
		printf("failed...\n");
		kill(mutateePid, SIGKILL);
		int status;
		wait(&status);
		return 0;
	}

	BPatch_image *image = proc->getImage();
	vector<BPatch_function *> functions, tp;

	proc->loadLibrary("./dyn.so");
	image->findFunction("quebec", functions);
	image->findFunction("tp1", tp);
	image->findFunction("tp2", tp);

	vector<BPatch_snippet *> args;
	BPatch_funcCallExpr f1(*(tp[0]), args);
	vector<BPatch_point *> *points = functions[0]->findPoint(BPatch_entry);
	proc->insertSnippet(f1, points[0]);

	BPatch_funcCallExpr f2(*(tp[1]), args);
	proc->insertSnippet(f2, points[0]);
	BPatch_funcCallExpr f3(*(tp[1]), args);
	proc->insertSnippet(f3, points[0]);
	BPatch_funcCallExpr f4(*(tp[1]), args);
	proc->insertSnippet(f4, points[0]);
	BPatch_funcCallExpr f5(*(tp[1]), args);
	proc->insertSnippet(f5, points[0]);
	proc->continueExecution();
}
