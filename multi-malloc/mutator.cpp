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
void instrument_process(BPatch_process *proc, int type)
{
	BPatch_image *image = proc->getImage();
	vector<BPatch_function *> functions, tp;
	int a = 12;

	proc->loadLibrary("./dyn.so");
	BPatch_variableExpr *var1,*var2;
	vector<BPatch_snippet *> args;
	if(type == 0){
		image->findFunction("tp1", tp);
		image->findFunction("foo", functions);
		var1 = proc->malloc(4);
		var1->writeValue(&a, sizeof(int), false);
		args.push_back(new BPatch_constExpr(var1->getBaseAddr()));
	}
	else
	{
		image->findFunction("tp2", tp);
		image->findFunction("foo", functions);
		var2 = proc->malloc(4);
		var2->writeValue(&a, sizeof(int), false);
		args.push_back(new BPatch_constExpr(var2->getBaseAddr()));
	}

	BPatch_funcCallExpr f1(*(tp[0]), args);
	vector<BPatch_point *> *points = functions[0]->findPoint(BPatch_entry);
	proc->insertSnippet(f1, points[0]);

}

int main (int argc, const char* argv[]) {
	BPatch bpatch1, bpatch2;

	int mutateePid = fork();

	if(mutateePid == 0)
	{
		char *args[2] = {NULL};
		args[0] = (char*) "./mutatee";
		execvp("./mutatee", args);
	}

	sleep(1);

	BPatch_process *proc1 = bpatch1.processAttach(NULL, mutateePid);
	instrument_process(proc1, 0);
	proc1->continueExecution();
	//proc1->detach(true);

	sleep(1);

	proc1->stopExecution();
//	BPatch_process *proc2 = bpatch2.processAttach(NULL, mutateePid);
	instrument_process(proc1, 1);
	proc1->continueExecution();

	while (!proc1->isTerminated())
		bpatch1.waitForStatusChange();
	proc1->detach(true);
	return 0;

}
