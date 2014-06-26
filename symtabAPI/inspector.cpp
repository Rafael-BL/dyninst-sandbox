#include <Symtab.h>
#include <Function.h>
#include <BPatch.h>
#include <BPatch_object.h>
#include <AddrLookup.h>
#include <unistd.h>
#include <vector>

using namespace Dyninst;
using namespace SymtabAPI;

int main()
{

	std::vector <Symbol *> syms;
	std::vector <Function *> funcs;
	BPatch bpatch;
	BPatch_process *proc = bpatch.processCreate("../suspect", NULL);
	std::vector<BPatch_object *> objs;
	proc->getImage()->getObjects(objs);
	Symtab *sym = SymtabAPI::convert(objs[0]);

	std::vector<localVar *> params;
	typeEnum te;
	if (sym->findFunctionsByName(funcs, "print"))
	{
		funcs[0]->getParams(params);
		for(std::vector<localVar *>::iterator iter = params.begin(); iter != params.end(); iter++)
		{
			std::string s = (*iter)->getType()->getName();
			std::cout<<(*iter)->getType()->getName()<<": "<<(*iter)->getName()<<" : "<<(*iter)->getType()->getSize()<<std::endl;
		}
	}

	return 0;
}
