#include "fileofflinereader.h"
#include "support.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include "samplegen.h"
#include <vector>

#include "commandline.h"

// In the following cN is not used in estimator-nosplitting
void exp_uniform( int k, int n_step, int n_cnt, double cL, double cp, double cN ); 
void exp_discrete( std::vector<double> &p, int pmin_inv, int n_step, int n_cnt, double cL, double cp, double cN );
void exp_file( std::string filename, int pmin_inv, int n_step, int n_cnt, double cL, double cp, double cN );

int main(int argc, char *argv[])
{
    SampleGen gen;
    gen.reset();
    gen.setSeed( 0 );
    gen.uniform( 20, 100 );
    std::shared_ptr< const std::map<int, int> > pHist = gen.getHist();
    for ( std::map<int,int>::const_iterator it = pHist->begin(); it != pHist->end(); ++it )
    {
        std::cout<<it->first<<"\t"<<it->second<<std::endl;
    }
    std::cout<<std::endl;
    std::shared_ptr< const std::map<int, int> > pFin = gen.getFin();
    for ( std::map<int,int>::const_iterator it = pFin->begin(); it != pFin->end(); ++it )
    {
        std::cout<<it->first<<"\t"<<it->second<<std::endl;
    }
    
	// double cL = 0.6, cp = 1, cN = 0.5;
	// int ExpType = 0;
	// std::CommandLine cmd;
	// cmd.AddValue ("cL",  "L=cL log k", cL);
	// cmd.AddValue ("cp",  "rEnd=cp log k/n", cp);
	// cmd.AddValue ("exp",  "0=uniform,1=mix,2=hamlet", ExpType);
	// cmd.Parse (argc, argv);

	// switch (ExpType)
	// {
	// case 0:
	// {
	// 	/* ----------------------UNIFORM---------------------------- */
	// 	int k = 1000000;
	// 	exp_uniform(k, 50000, 20, cL, cp, cN);
	// 	/* ----------------------END UNIFORM---------------------------- */
	// }
	// break;
	
	// case 1:
	// {
	// 	/* ----------------------DISCRETE---------------------------- */
	// 	int k = 1000000;
	// 	std::vector<double> p( k );
	// 	p[0] = 0.5;
	// 	for (int i = 1; i < k; i++)
	// 		p[i] = 0.5 / (k-1);
	
	// 	int pmin_inv = 2*(k-1);
	// 	exp_discrete( p, pmin_inv, 50000, 20, cL, cp, cN);
	// 	/* ----------------------END DISCRETE---------------------------- */
	// }
	// break;

	// case 2:
	// {
	// 	/* ----------------------FILE---------------------------- */
	// 	std::string filename = "hamlet.txt";
	// 	int pmin_inv = 32000;
	// 	exp_file( filename, pmin_inv, 1000, 32, cL, cp, cN);
	// 	/* ----------------------END FILE---------------------------- */
	// }
	// break;
	// default:
	// 	break;
	// }



    
	// ----------------------TEST ON HAMLET ---------------------------- //
	// Support<int> support( 1000000 );
	// support.setCL(0.8);
	// support.setCP(1);
	// support.setN(200000);
	// int L = support.getL();
	// // std::cout<<L<<std::endl;
	// for (int i=0; i <= L+3; i++)
	// 	std::cout<<i<<"\t"<<support.getCoeff2(i)<<std::endl;
	
	// {
	// 	int k = 1000000;
	// 	Support<std::string> support( k );
	// 	support.setCP(1);
	// 	support.setCL(0.8);

		
	// 	std::shared_ptr<std::map<int, int> > fin(new std::map<int, int>);
		
	// 	std::ifstream infile;
	// 	infile.open( "../Shake-parse/fin-spe.txt" );
	// 	for (int i = 0; i < 100; ++i)
	// 	{
	// 		int j, fj;
	// 		infile >> j >> fj;
	// 		fin->insert( std::pair<int,int>( j,fj ) );
	// 		// std::cout<<j<<" "<<fj<<std::endl;
	// 	}
	// 	infile.close();
		
	// 	support.setFin( fin );
	// 	std::cout<< std::fixed
	// 			 << (int)support.estimate_plug() + 846
	// 			 << "\t"<< (int)support.estimate() + 846
	// 			 << "\t"<< (int)support.estimate2() + 846
	// 			 // << "\t"<< (int)support.estimate_TG()
	// 			 // << "\t"<< (int)support.estimate_CL1()
	// 			 // << "\t"<< (int)support.estimate_CL2()
	// 			 << std::endl;
	// }
	// ----------------------END TEST ON HAMLET ---------------------------- //

    return 0;
}



void exp_file( std::string filename, int pmin_inv, int n_step, int n_cnt, double cL, double cp, double cN )
{
	FileOfflineReader f( filename );
	f.reset();
	
	int truth = f.distinctTotal();

	Support<std::string> support( pmin_inv ); // set 1/p_min
	support.setCP(cp); // Approximation interval is [1/k, min{ 1, c_p * log(k) / n }]
	support.setCL(cL); // L = c_L * log(k) and plug-in if N>L
	support.setCN(cN);
	
	std::cout<< "n \ttruth \tplug \tpoly \tpoly2 \tTG \tCL1 \tCL2"<< std::endl;
	for ( int i = 0; i < n_cnt; i++)
	{
		f.randread( n_step );
		support.setHist( f.getHist() );
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
}


void exp_uniform( int k, int n_step, int n_cnt, double cL, double cp, double cN )
{
	SampleGen gen;
	gen.reset();
	gen.setSeed( 0 );
	
	int truth = k;

	Support<int> support( k ); 
	support.setCP(cp); // Approximation interval is [1/k, min{ 1, c_p * log(k) / n }]
	support.setCL(cL); // L = c_L * log(k) and plug-in if N>L
	support.setCN(cN);


	std::cout<< "n \ttruth \tplug \tpoly \tpoly2 \tsinc \tTG \tCL1 \tCL2"<< std::endl;
	for ( int i = 0; i < n_cnt; i++)
	{
		gen.uniform( n_step, k );
		support.setHist( gen.getHist() );
		std::cout<< std::fixed
				 << support.getN()
				 << "\t" << truth
				 << "\t"<< (int)support.estimate_plug()
				 << "\t"<< (int)support.estimate()
				 << "\t"<< (int)support.estimate2()
				 << "\t"<< (int)support.estimate_sinc()
				 << "\t"<< (int)support.estimate_TG()
				 << "\t"<< (int)support.estimate_CL1()
				 << "\t"<< (int)support.estimate_CL2()
				 << std::endl;
	}
}


void exp_discrete( std::vector<double> &p, int pmin_inv, int n_step, int n_cnt, double cL, double cp, double cN )
{
	SampleGen gen;
	gen.reset();
	gen.setSeed( 0 );
	
	int truth = p.size();

	Support<int> support( pmin_inv ); // set 1/p_min
	support.setCP(cp); // Approximation interval is [1/k, min{ 1, c_p * log(k) / n }]
	support.setCL(cL); // L = c_L * log(k) and plug-in if N>L
	support.setCN(cN);

	std::cout<< "n \ttruth \tplug \tpoly \tpoly2 \tsinc \tTG \tCL1 \tCL2"<< std::endl;
	for ( int i = 0; i < n_cnt; i++)
	{
		gen.discrete( n_step, &p );
		support.setHist( gen.getHist() );
		std::cout<< std::fixed
				 << support.getN()
				 << "\t" << truth
				 << "\t"<< (int)support.estimate_plug()
				 << "\t"<< (int)support.estimate()
				 << "\t"<< (int)support.estimate2()
				 << "\t"<< (int)support.estimate_sinc()
				 << "\t"<< (int)support.estimate_TG()
				 << "\t"<< (int)support.estimate_CL1()
				 << "\t"<< (int)support.estimate_CL2()
				 << std::endl;
	}
}


