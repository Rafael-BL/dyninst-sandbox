#include <BPatch.h>
#include <BPatch_function.h>
#define BIN_PATH  "/home/frdeso/projets/dyninst-sandbox/suspect"
int main()
{
	BPatch bpatch;

	BPatch_binaryEdit *proc = bpatch.openBinary(BIN_PATH);
	BPatch_image *image = proc->getImage();
	vector<BPatch_function*> *fcts = image->getProcedures(false);
	fcts->clear();
	std::vector<BPatch_module *> *mods = image->getModules();
	char buff[256];
	for(auto i: *mods)
	{
		i->getName(buff,256);
		if(strcmp(buff,"DEFAULT_MODULE") == 0)
		{
			fcts = i->findFunction("b*", *fcts);
			for(auto j: *fcts)
			{
				vector<BPatch_localVar *> *params = j->getParams();
				cout<<""<<j->getName()<<"(";
				for(unsigned int i = 0; i < params->size(); ++i )
				{

					//Push the type of the next argument
					switch((*params)[i]->getType()->getDataClass())
					{
					case BPatch_dataScalar:
					    cout<<(*params)[i]->getType()->getName()<<" "<<(*params)[0]->getName();
					    break;
					case BPatch_dataPointer:
					    cout<<(*params)[i]->getType()->getConstituentType()->getName()<<" *"<<(*params)[0]->getName();
					    break;
					default:
					    cerr<<"unknown type"<<endl;
					    exit(-1);
					    break;
					}
					if(i < params->size()-1)
						cout<<", ";
				}
				cout<<")"<<endl;
			}
		}
	}
	return 0;
}
