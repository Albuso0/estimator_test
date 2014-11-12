#include "fileparser.h"
#include "support.h"
#include <iostream>


int main(int argc, char *argv[])
{
	FileParser f("hamlet.txt");
	std::map<std::string,int>* hist;
	// hist = f.hist();
	// for ( std::map<std::string,int>::iterator it = hist->begin(); it != hist->end(); ++it )
	// 	std::cout<< it->first<<"  " << it->second<<std::endl;
	// std::cout<< hist->size()<<std::endl;
	// delete hist;

	f.reloadFile();
	f.hist(6000);
	hist = f.getHist();
	std::cout<< hist->size()<<std::endl;

	delete hist;
	std::cout<<f.wordCounted()<<std::endl;
	
    return 0;
}

