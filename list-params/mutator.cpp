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
	BPatch_binaryEdit *bin = bpatch.openBinary("./mutatee");

	BPatch_image *image = bin->getImage();
	vector<BPatch_function *> print_fcts;
	image->findFunction("foo", print_fcts);


	vector<BPatch_localVar *> *params = print_fcts[0]->getParams();

	cout<<params->size()<<" formal parameter(s) found."<<endl;

	for(int i = 0; i < params->size(); ++i)
	{
		cout<<i<<" : "<<(*params)[i]->getName()<<endl;
	}
	return 0;
}
