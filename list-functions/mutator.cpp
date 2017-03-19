#include <BPatch.h>
#include <BPatch_function.h>
#define BIN_PATH  "/home/rafael/dyninst-sandbox/suspect"
int main(int argc, char **argv)
{

	if(argc <= 1)
	{
		std::cout<<"No binary to examine. Exiting..."<<std::endl;
		exit(-1);
	}
	char *path = argv[1];
	BPatch bpatch;


	BPatch_binaryEdit *proc = bpatch.openBinary(path);
	BPatch_image *image = proc->getImage();
	std::vector<BPatch_function*> *fcts = image->getProcedures(false);
	fcts->clear();
	std::vector<BPatch_module *> *mods = image->getModules();
	char buff[256];
	if(mods->size() == 0){
	    std::cerr<<"No module found"<<std::endl;
	    exit(0);
	}
	std::cout<<"mods "<<mods->size()<<std::endl;

	for(auto i: *mods)
	{
		i->getName(buff,256);
		//if(strcmp(buff,"DEFAULT_MODULE") == 0)
		{
			//cout<<"*"<<buff<<endl;
			fcts = i->getProcedures();
	    	    	if(fcts->size() == 0){
	    	    		std::cerr<<buff<<": No function found"<<std::endl;
	    	    		continue;
	    	    	}
			std::cout<<"fcts "<<fcts->size()<<std::endl;
			for(auto j: *fcts)
			{
				std::vector<BPatch_localVar *> *params = j->getParams();
				std::cout<<"("<<buff<<") "<<j->getName()<<"(";
				for(unsigned int i = 0; i < params->size(); ++i )
				{
					//Push the type of the next argument
					switch((*params)[i]->getType()->getDataClass())
					{
					case BPatch_dataScalar:
					    std::cout<<(*params)[i]->getType()->getName()<<" "<<(*params)[i]->getName();
					    break;
					case BPatch_dataPointer:
					    std::cout<<(*params)[i]->getType()->getConstituentType()->getName()<<" *"<<(*params)[i]->getName();
					    break;
					case BPatch_dataStructure:
					    std::cout<<"struct "<< (*params)[i]->getName();
					    break;
					default:
					    std::cout<<(*params)[i]->getType()->getName()<<" " <<(*params)[i]->getName();
					    break;
					}
					if(i < params->size()-1)
						std::cout<<", ";
				}
				std::cout<<")"<<std::endl;
			}
			fcts->clear();
		}
	}
	return 0;
}
