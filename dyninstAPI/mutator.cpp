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

	switch(type)
	{
		case 1:
		{
		    /* Adding function call to a shared object */
		    proc->loadLibrary(DYNSO_PATH);
			std::vector<BPatch_function *> print_fcts, tp_fcts;
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
			std::vector<BPatch_function *> answer_fcts;
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
			break;
		}
		case 3:
		{
			//retreive parameter type
			std::vector<BPatch_function *> print_fcts;
			image->findFunction("print", print_fcts);

			points = print_fcts[0]->findPoint(BPatch_entry);
			std::vector<BPatch_snippet *> writeArgs;

			std::vector<BPatch_localVar *> *params = print_fcts[0]->getParams();
			std::vector<BPatch_variableExpr *> expr;
			std::vector<BPatchSnippetHandle *> snippetHandle;
			for(unsigned int i = 0; i < params->size(); ++i )
			{	
				BPatch_variableExpr *paramValue =
						proc->malloc(*((*params)[i]->getType()));

				expr.push_back(paramValue);
				BPatch_paramExpr param(i);

				BPatch_arithExpr saveParam(BPatch_assign,
											*paramValue,
											param);

				snippetHandle.push_back(proc->insertSnippet(saveParam, points[0]));
			}
	
			proc->continueExecution();
			sleep(2);
			proc->stopExecution();

			for(unsigned int i = 0; i < params->size(); ++i )
			{
				switch((*params)[i]->getType()->getDataClass())
				{
					case BPatch_dataScalar:
					{
						unsigned int a = 0;
						expr[i]->readValue(&a);
						std::cout<<"Scalar parameter "<<i<<" at "<<a<<std::endl;
						std::cout<<(*params)[i]->getType()->getName()<<std::endl;
						break;
					}
					case BPatch_dataPointer:
					{
						int *a = 0;
						expr[i]->readValue(&a);
						std::cout<<"Pointer parameter "<<i<<" at "<<a<<std::endl;
						std::cout<<(*params)[i]->getType()->getConstituentType()->getName()<<std::endl;
						break;
					}
					default:
					{
						std::cout<<"Dataclass unsupported"<<std::endl;
						break;
					}
				}
			}
			proc->continueExecution();

			for(auto s: snippetHandle)
			{
				proc->deleteSnippet(s);
			}
			break;
		}
		case 4:
		{
		    proc->loadLibrary(DYNSO_PATH);

			std::vector<BPatch_function *> awesome_fcts, tp_fcts;
		    image->findFunction("print", awesome_fcts);
		    if(awesome_fcts.size() <= 0)
		    {
				std::cerr<<"Function to instrument not found"<<std::endl;
		    }
		    image->findFunction("tpvarargs", tp_fcts);

		    if(tp_fcts.size() <= 0)
		    {
		    	std::cerr<<"Function to call not found"<<std::endl;
		    }

		    std::vector<BPatch_snippet*> args;
		    //Push all the argument in the vector
		    std::vector<BPatch_localVar *> *params = awesome_fcts[0]->getParams();
		    //Push number of argument
		    args.push_back(new BPatch_constExpr(params->size()));
			for(unsigned int i = 0; i < params->size(); ++i )
			{
				//Push the type of the next argument
				switch((*params)[i]->getType()->getDataClass())
				{
					case BPatch_dataScalar:
					{
						std::string typeName = (*params)[i]->getType()->getName();
						if(typeName == "char")
						{
							args.push_back(new BPatch_constExpr(CHAR));
						}
						else if (typeName == "short int")
						{
							args.push_back(new BPatch_constExpr(SHORT_INT));
						}
						else
						{
							args.push_back(new BPatch_constExpr(INT));
						}
						break;
					}
					case BPatch_dataPointer:
					{
						std::string typeName = (*params)[i]->getType()->getConstituentType()->getName();
						if(typeName == "char")
						{
							args.push_back(new BPatch_constExpr(CHAR_PTR));
						}
						else if (typeName == "short int")
						{
							args.push_back(new BPatch_constExpr(SHORT_INT_PTR));
						}
						else
						{
							args.push_back(new BPatch_constExpr(INT_PTR));
						}
						break;
					}
					default:
					{
						std::cout<<"Dataclass unsupported"<<std::endl;
						args.push_back(new BPatch_constExpr(UNKNOWED_TYPE));
						break;
					}
				}
				//Push the next argument
				args.push_back(new BPatch_paramExpr(i));
			}
		    BPatch_funcCallExpr call_tp(*tp_fcts[0], args);

			points = awesome_fcts[0]->findPoint(BPatch_entry);
			BPatchSnippetHandle* snippetHandle =
						proc->insertSnippet(call_tp, points[0] );
			proc->continueExecution();
			sleep(2);

			proc->deleteSnippet(snippetHandle);
			break;
		}
		default:
			break;

	}

	while (!proc->isTerminated()) {
		bpatch.waitForStatusChange();
	}
	proc->detach(true);
	return 0;
}
