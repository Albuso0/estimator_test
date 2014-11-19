#include "filereader.h"
#include "fileofflinereader.h"
#include "support.h"
#include <iostream>
#include <algorithm>


int main(int argc, char *argv[])
{
	FileReader fr("Shakespeare.txt");
	int wordT = fr.wordTotal();
	fr.reset();
	FileReader::const_HistPtr Hist = fr.read(wordT);
	int distinctT = Hist->size();
	std::cout<<wordT<<"\t"<<distinctT<<std::endl;
	
	Support<std::string> support(32000);
	support.setCN(0.7); // Plug-in if N_i >= c_N * log(k)
	support.setCP(1.0); // Approximation interval is [1/k, min{ 1, c_p * log(k) / n }]
	support.setCL(1.1); // L = c_L * log(k)

	fr.reset();
	
	
	
	
	// std::cout<<"Words"<<"\t"<<"Truth"<<"\t"<<"Esti"<<"\t"<<"Plug"<<std::endl;
	// Test read(N) or randread(N)
	// for ( int i = 0; i < 32; i++ )
	// {
	// 	Hist = fr.read(1000);
	// 	support.setHist( Hist );
	// 	std::cout<<fr.wordN()<<"\t"<<fr.distinctTotal()<<"\t"<<support.estimate()<<"\t"<<support.estimate_plug()<<std::endl;
	// }

	// Test read(vector)
	// std::vector<int> pos( fr.wordTotal() );
	// for (int i = 0; i < fr.wordTotal(); ++i) pos[i] = i;
	// std::random_shuffle ( pos.begin(), pos.end() );
	// std::vector<int>::iterator it = pos.begin();
	// for ( int i = 0; i < 32; i++ )
	// {
	// 	std::vector<int>::iterator end = ( it + 1000 > pos.end() ) ? pos.end() :(it + 1000) ;
	// 	if ( it < end )
	// 	{
	// 		std::vector<int> PosVec ( it, end );
	// 		Hist = fr.read(PosVec);
	// 		support.setHist( Hist );
	// 		it += 1000;
	// 		std::cout<<fr.wordN()<<"\t"<<fr.distinctTotal()<<"\t"<<support.estimate()<<"\t"<<support.estimate_plug()<<std::endl;
	// 	}
	// }
	

    return 0;
}

