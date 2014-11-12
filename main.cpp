#include "fileparser.h"
#include "support.h"
#include <iostream>


int main(int argc, char *argv[])
{
	FileParser f("hamlet.txt");
	Support<std::string> support(32000);
	
	
	// std::cout<< f.distinctWordCount() <<std::endl;

	// f.reloadFile();
	// support.updateHist( f.incHist(6000) );
	// std::cout<<support.estimate()<<std::endl;

    return 0;
}

