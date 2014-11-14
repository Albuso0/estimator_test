#include "filereader.h"
#include "fileofflinereader.h"
#include "support.h"
#include <iostream>


int main(int argc, char *argv[])
{
	FileOfflineReader fr("hamlet.txt");
	Support<std::string> support(32000);
	
	FileReader::const_HistPtr Hist;
	fr.reset();
	for ( int i = 0; i < 32; i++ )
	{
		Hist = fr.randread(1000);
		support.setHist( Hist );
		std::cout<<fr.wordN()<<"\t"<<support.estimate()<<"\t"<<fr.distinctTotal()<<"\t"<<support.estimate_plug()<<std::endl;
	}
	// fr.reset();
	// FileReader::const_HistPtr Hist = fr.randread(32000);
	// support.setHist( Hist );
	// std::cout<<"Truth: "<<fr.distinctTotal()<<std::endl;
	// std::cout<<"Naive: "<<support.estimate_plug()<<std::endl;
	// std::cout<<"Estimate: "<<support.estimate()<<std::endl;
	
	// int maxCnt = 0;
	// for (std::map<std::string, int>::const_iterator it = pHist->begin(); it != pHist->end(); ++it )
	// {
	// 	if ( it->second > maxCnt ) maxCnt = it->second;
	// }
	// std::cout<<maxCnt<<" "<<std::endl;
	

    return 0;
}

