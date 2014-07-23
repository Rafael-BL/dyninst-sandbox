#include <BPatch.h>
#include <BPatch_point.h>
#include <BPatch_function.h>
#include <stdio.h>
#include <unistd.h>


int main (int argc, const char* argv[]) {
	BPatch bpatch;
	BPatch_process *handle = bpatch.processCreate("./mutatee", NULL);

	BPatch_image *image = handle->getImage();

	vector<BPatch_function*> functions;
	std::vector<BPatch_snippet *> args;

	image->findFunction("printf", functions);

	BPatch_snippet *fmt = new BPatch_constExpr("oneTimeCodeAsync\n");
	args.push_back(fmt);
	BPatch_funcCallExpr firstPrintfCall(*(functions[0]), args);
	handle->oneTimeCodeAsync(firstPrintfCall);
	
	args.clear();

	fmt = new BPatch_constExpr("oneTimeCode\n");
	args.push_back(fmt);
	BPatch_funcCallExpr secondPrintfCall(*(functions[0]), args);
	handle->oneTimeCode(secondPrintfCall, NULL);
	
	printf("Mutator: continueExecution\n");
	handle->continueExecution();

	while (!handle->isTerminated())
		bpatch.waitForStatusChange();
	return 0;
}
