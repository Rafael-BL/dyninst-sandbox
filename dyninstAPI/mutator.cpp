#include <BPatch.h>
#include <BPatch_point.h>
#include <BPatch_function.h>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
int main (int argc, const char* argv[]) {
    BPatch bpatch;
    //BPatch_process *proc = bpatch.processCreate(argv[2], argv + 2);

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
	bpatch.setTrampRecursive(true);
    bpatch.setSaveFPR(false);

    BPatch_object *ipa = proc->loadLibrary(argv[1]);
    BPatch_image *image = proc->getImage();

    std::vector<BPatch_function *> foo_fns, ipa_fns;
    image->findFunction("print", foo_fns);
    image->findFunction("tp", ipa_fns);

    std::vector<BPatch_snippet*> args;
    BPatch_funcCallExpr call_ipa(*ipa_fns[0], args);
	BPatchSnippetHandle* snippetHandle =
			 proc->insertSnippet(call_ipa,
					(foo_fns[0]->findPoint(BPatch_entry))[0]);

    proc->continueExecution();
	sleep(2);
	
	proc->deleteSnippet(snippetHandle);
    while (!proc->isTerminated()) {
      bpatch.waitForStatusChange();
    }

	proc->detach(true);
    return 0;
}
