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
	char *args[3] = {NULL};
	args[0] = "../suspect";
	args[1] = "1";
	execvp("../suspect", args);
	//	args[0] = "/usr/lib/firefox/firefox";
	//	execvp("/usr/lib/firefox/firefox", args);
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
    vector<BPatch_point *> *points;

    switch(type)
    {
	case 1:
	    {
		/* Adding function call to a shared object */
		proc->loadLibrary(DYNSO_PATH);
		vector<BPatch_function *> print_fcts, tp_fcts;
		image->findFunction("print", print_fcts);
		image->findFunction("tp", tp_fcts);

		vector<BPatch_snippet*> args;
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
		vector<BPatch_function *> answer_fcts;
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
		cout<<"getAnswer call counter: "<<a<<endl;
		proc->deleteSnippet(snippetHandle);
		break;
	    }
	case 3:
	    {
		//retreive parameter type
		vector<BPatch_function *> print_fcts;
		image->findFunction("print", print_fcts);

		points = print_fcts[0]->findPoint(BPatch_entry);
		vector<BPatch_snippet *> writeArgs;

		vector<BPatch_localVar *> *params = print_fcts[0]->getParams();
		vector<BPatch_variableExpr *> expr;
		vector<BPatchSnippetHandle *> snippetHandle;

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
				cout<<"Scalar parameter "<<i<<" at "<<a<<endl;
				cout<<(*params)[i]->getType()->getName()<<endl;
				break;
			    }
			case BPatch_dataPointer:
			    {
				int *a = 0;
				expr[i]->readValue(&a);
				cout<<"Pointer parameter "<<i<<" at "<<a<<endl;
				cout<<(*params)[i]->getType()->getConstituentType()->getName()<<endl;
				break;
			    }
			default:
			    {
				cout<<"Dataclass unsupported"<<endl;
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

		vector<BPatch_function *> awesome_fcts, tp_fcts;
		image->findFunction("print", awesome_fcts);
		if(awesome_fcts.size() <= 0)
		{
		    cerr<<"Function to instrument not found"<<endl;
		}
		image->findFunction("tpvarargs", tp_fcts);

		if(tp_fcts.size() <= 0)
		{
		    cerr<<"Function to call not found"<<endl;
		}

		vector<BPatch_snippet*> args;
		//Push all the argument in the vector
		vector<BPatch_localVar *> *params = awesome_fcts[0]->getParams();
		//Push number of argument
		args.push_back(new BPatch_constExpr(params->size()));
		for(unsigned int i = 0; i < params->size(); ++i )
		{
		    //Push the type of the next argument
		    switch((*params)[i]->getType()->getDataClass())
		    {
			case BPatch_dataScalar:
			    {
				string typeName = (*params)[i]->getType()->getName();
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
				string typeName = (*params)[i]->getType()->getConstituentType()->getName();
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
				cout<<"Dataclass unsupported"<<endl;
				args.push_back(new BPatch_constExpr(UNKNOWED_TYPE));
				break;
			    }
		    }
		    //Push name of the parameter
		    args.push_back(new BPatch_constExpr((*params)[i]->getName()));
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
	case 5:
	    {
		proc->loadLibrary(DYNSO_PATH);

		vector<BPatch_function *> awesome_fcts, tp_fcts;
		image->findFunction("sherbrook", awesome_fcts);
		if(awesome_fcts.size() <= 0)
		{
		    cerr<<"Function to instrument not found"<<endl;
		}

		vector<BPatch_snippet*> args;
		//get the arguments in the vector
		vector<BPatch_localVar *> *params = awesome_fcts[0]->getParams();
		if(params->size() <= 0)
		{
		    cerr<<"No Params found"<<endl;
		}

		for(unsigned int i = 0; i < params->size(); ++i )
		{
		    //Push the type of the next argument
		    switch((*params)[i]->getType()->getDataClass())
		    {
			case BPatch_dataScalar:
			    {
				string typeName = (*params)[i]->getType()->getName();
				cout<<"Sca: "<<typeName<<endl;
				if(typeName == "int")
				{
				    image->findFunction("tp_print_param_int",tp_fcts);
				    args.push_back(new BPatch_constExpr((*params)[i]->getName()));
				}
				else if(typeName == "char")
				{
				    image->findFunction("tp_print_param_char",tp_fcts);
				    args.push_back(new BPatch_constExpr((*params)[i]->getName()));
				}
				break;
			    }
			case BPatch_dataPointer:
			    {
				string typeName = (*params)[i]->getType()->getConstituentType()->getName();
				cout<<"ptr: "<<typeName<<endl;
				if(typeName == "char")
				{
				    image->findFunction("tp_print_param_char_ptr",tp_fcts);
				    args.push_back(new BPatch_constExpr((*params)[i]->getName()));
				}
				else if(typeName == "void")
				{
				    image->findFunction("tp_print_param_void_ptr",tp_fcts);
				    args.push_back(new BPatch_constExpr((*params)[i]->getName()));
				}
				break;
			    }
			default:
			    {
				cerr<<"unknown type"<<endl;
				exit(-1);
				break;
			    }
		    }

		    args.push_back(new BPatch_paramExpr(i));
		    BPatch_funcCallExpr call_tp(*tp_fcts[0], args);
		    points = awesome_fcts[0]->findPoint(BPatch_entry);
		    BPatchSnippetHandle* snippetHandle =
			proc->insertSnippet(call_tp, points[0] );
		    args.clear();
		    tp_fcts.clear();
		}

		proc->continueExecution();
		sleep(1);
		break;
	    }
	case 6:
	{
		proc->loadLibrary(DYNSO_PATH);

		vector<BPatch_function *> awesome_fcts, tp_fcts;
		image->findFunction("sherbrook", awesome_fcts);
		if(awesome_fcts.size() <= 0)
		{
			cerr<<"Function to instrument not found"<<endl;
		}

		vector<BPatch_snippet*> args;
		//get the arguments in the vector
		vector<BPatch_localVar *> *vars = awesome_fcts[0]->getParams();
		if(vars->size() <= 0)
		{
			cerr<<"No vars found"<<endl;
		}

		for(unsigned int i = 0; i < vars->size(); ++i )
		{
			cout<<i<<" : "<<(*vars)[i]->getName()<<endl;
		}
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
