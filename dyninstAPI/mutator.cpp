#include <BPatch.h>
#include <BPatch_point.h>
#include <BPatch_function.h>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define DYNSO_PATH "./dyn.so"
int main (int argc, const char* argv[]) {
		BPatch bpatch;
	//BPatch_process *proc = bpatch.processCreate(argv[2], argv + 2);
	int type = atoi(argv[1]);
	int mutateePid = fork();

	if(mutateePid == 0)
	{
		char *args[2] = {NULL};
		args[0] = "../suspect";

		execvp("../suspect", args);
	}

	sleep(2);
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
	std::vector<BPatch_point *> *points;
	std::vector<BPatch_function *> print_fcts, tp_fcts, answer_fcts;

	switch(type)
	{
		case 1:
		{
		    /* Adding function call to a shared object */
		    BPatch_object *api = proc->loadLibrary(DYNSO_PATH);

		    image->findFunction("print", print_fcts);
		    image->findFunction("tp", tp_fcts);

		    std::vector<BPatch_snippet*> args;
		    BPatch_funcCallExpr call_tp(*tp_fcts[0], args);

			points = print_fcts[0]->findPoint(BPatch_entry);
			BPatchSnippetHandle* snippetHandle =
						proc->insertSnippet(call_tp, points[0] );
			proc->continueExecution();
			sleep(2);

			proc->deleteSnippet(snippetHandle);
			break;
		}
		case 2:
		{
			//
			image->findFunction("getAnswer", answer_fcts);
			points = answer_fcts[0]->findPoint(BPatch_entry);
			BPatch_variableExpr *intCounter =
						proc->malloc(*(image->findType("int")));
			BPatch_arithExpr addOne(BPatch_assign, *intCounter,
							BPatch_arithExpr(BPatch_plus, *intCounter,
							BPatch_constExpr(1)));

			BPatchSnippetHandle* snippetHandle =
							proc->insertSnippet(addOne, points[0]);

			proc->continueExecution();
			sleep(2);
			int a = 0;
			proc->stopExecution();
			intCounter->readValue(&a);
			proc->continueExecution();
			std::cout<<"getAnswer call counter: "<<a<<std::endl;
			proc->deleteSnippet(snippetHandle);
		}
			break;
		default:
			break;

	}

	while (!proc->isTerminated()) {
		bpatch.waitForStatusChange();
	}
	proc->detach(true);
	return 0;
}
