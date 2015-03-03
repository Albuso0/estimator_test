#include "fileofflinereader.h"
#include "support.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include "samplegen.h"
#include <vector>



int main(int argc, char *argv[])
{

	
	// FileOfflineReader f("hamlet.txt");
	// std::cout<<f.distinctTotal()<<std::endl;
	
	SampleGen gen;
	gen.reset();
	gen.setSeed( 0 );
	
	/* ----------------------UNIFORM---------------------------- */
	// int k = 1000000;
	// int truth = k;
	// int pmin_inv = k;
	/* ----------------------END UNIFORM---------------------------- */


	/* ----------------------EMPERICAL NEGATIVE BINOMIAL---------------------------- */
	// SampleGen emp;
	// emp.reset();
	// emp.setSeed( 0 );
	// int k = 1000000;
	// emp.negative_binomial( k, 4, 0.04 );
	// SampleGen::const_HistPtr hist = emp.getHist();
	// int truth = hist->size();
	// std::vector<double> p( truth );
	// int cnt = 0;
	// for ( std::map<int,int>::const_iterator it = hist->begin(); it != hist->end(); ++it )
	// {
	// 	p[cnt] = it->second;
	// 	++cnt;
	// }
	// int pmin_inv = k;
	/* ----------------------END EMPERICAL NEGATIVE BINOMIAL---------------------------- */

	
	/* ----------------------TRUNCATED POISSON---------------------------- */
	// gen.Poisson_truncated( 300, l, 0 , 200 );
	// Support<int> support(int(exp(10)));
	// std::cout<<"Truth: "<< 20 << std::endl;
	/* ----------------------END TRUNCATED POISSON---------------------------- */

	
	/* ----------------------DISCRETE---------------------------- */
	int k = 1000000;
	std::vector<double> p( k );
	p[0] = 0.5;
	for (int i = 1; i < k; i++)
		p[i] = 0.5 / (k-1);
	int truth = k;
	int pmin_inv = 2*(k-1);
	/* ----------------------END DISCRETE---------------------------- */




	
	/* ------------------SUPPORT EXP 1-------------------------------- */
	// Support<int> support( pmin_inv ); // set 1/p_min
	// support.setCN(0.5); // Plug-in if N_i >= c_N * log(k)
	// support.setCP(1); // Approximation interval is [1/k, min{ 1, c_p * log(k) / n }]
	// support.setCL(1); // L = c_L * log(k)
	
	// std::cout<< "n \ttruth \tplug \tpoly \tTG \tCL1 \tCL2"<< std::endl;
	// for ( int i = 0; i < 20; i++)
	// {
	// 	// gen.uniform( 50000, k );
	// 	gen.discrete( 50000, &p );
	// 	support.setHist( gen.getHist() );
	// 	std::cout<< std::fixed
	// 			 << support.getN()
	// 			 << "\t" << truth
	// 			 << "\t"<< (int)support.estimate_plug()
	// 			 << "\t"<< (int)support.estimate()
	// 			 << "\t"<< (int)support.estimate_TG()
	// 			 << "\t"<< (int)support.estimate_CL1()
	// 			 << "\t"<< (int)support.estimate_CL2()
	// 			 << std::endl;
	// }
	/* ------------------END SUPPORT EXP 1-------------------------------- */

	
	/* ------------------SUPPORT EXP 2-------------------------------- */
	Support<int> support( pmin_inv ); // set 1/p_min
	support.setCP(1); // Approximation interval is [1/k, min{ 1, c_p * log(k) / n }]
	support.setCL(1); // L = c_L * log(k) and plug-in if N>L
	
	std::cout<< "n \ttruth \tplug \tpoly \tpoly2 \tTG \tCL1 \tCL2"<< std::endl;
	for ( int i = 0; i < 20; i++)
	{
		/* ------------------DRAW NEW SAMPLES-------------------------------- */
		// gen.uniform( 50000, k );
		gen.discrete( 50000, &p );
		/* ------------------END DRAW NEW SAMPLES-------------------------------- */
		
		support.setHist( gen.getHist() );
		std::cout<< std::fixed
				 << support.getN()
				 << "\t" << truth
				 << "\t"<< (int)support.estimate_plug()
				 << "\t"<< (int)support.estimate()
				 << "\t"<< (int)support.estimate2()
				 << "\t"<< (int)support.estimate_TG()
				 << "\t"<< (int)support.estimate_CL1()
				 << "\t"<< (int)support.estimate_CL2()
				 << std::endl;
	}
	/* ------------------END SUPPORT EXP 2-------------------------------- */

		
	
	
    return 0;
}

