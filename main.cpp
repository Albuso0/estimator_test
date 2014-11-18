#include "filereader.h"
#include "fileofflinereader.h"
#include "support.h"
#include <iostream>


int main(int argc, char *argv[])
{
	FileOfflineReader fr("hamlet.txt");
	Support<std::string> support(32000);
	// support.setCN(0.5); // Plug-in if N_i >= c_N * log(k)
	// support.setCP(1.0); // Approximation interval is [1/k, min{ 1, c_p * log(k) / n }]
	// support.setCL(1.0); // L = c_L * log(k)
	
	FileOfflineReader::const_HistPtr Hist;
	fr.reset();
	std::cout<<"Words"<<"\t"<<"Truth"<<"\t"<<"Esti"<<"\t"<<"Plug"<<std::endl;
	for ( int i = 0; i < 32; i++ )
	{
		Hist = fr.randread(1000);
		support.setHist( Hist );
		std::cout<<fr.wordN()<<"\t"<<fr.distinctTotal()<<"\t"<<support.estimate()<<"\t"<<support.estimate_plug()<<std::endl;
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

