#include <BPatch.h>
#include <BPatch_point.h>
#include <BPatch_function.h>
#include <stdio.h>
#include <unistd.h>

#include "../../buche/buche.h"

int main (int argc, const char* argv[]) {
	BPatch bpatch;
	BPatch_process *handle = bpatch.processCreate("./mutatee", NULL);

	BPatch_image *image = handle->getImage();

	vector<BPatch_function*> functions;
	std::vector<BPatch_snippet *> args;

	BUCHE("a");
	image->findFunction("foo", functions);

	BPatch_variableExpr *expr = handle->malloc(sizeof(char) * 50);
	char arr[50] = "Good Morning";
	expr->writeValue((char*)arr, 50, false);
	args.push_back(new BPatch_constExpr(expr->getBaseAddr()));

	BPatch_funcCallExpr call(*(functions[0]), args);
	BUCHE("a");
	handle->oneTimeCodeAsync(call);

	BUCHE("a");
	printf("Mutator: continueExecution\n");
	handle->continueExecution();

	while (!handle->isTerminated())
		bpatch.waitForStatusChange();
	return 0;
}
