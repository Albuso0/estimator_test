#include "fileparser.h"
#include "support.h"
#include <iostream>


int main(int argc, char *argv[])
{
	FileParser f("hamlet.txt");
	FileParser::const_HistPtr pHist;
	
	pHist = f.hist();
	std::cout<< pHist->size()<<std::endl;

	f.reloadFile();
	pHist = f.hist(6000);
	// FileParser::HistPtr hist;
	// hist = f.hist();
	// for ( std::map<std::string,int>::iterator it = hist->begin(); it != hist->end(); ++it )
	// 	std::cout<< it->first<<"  " << it->second<<std::endl;
	// std::cout<< hist->size()<<std::endl;
	// delete hist;
	
	// f.reloadFile();
	// f.hist(6000);
	// hist = f.getHist();
	// std::cout<< hist->size()<<std::endl;
	// hist->clear();

	// std::cout<<f.wordCounted()<<std::endl;

	

    return 0;
}

