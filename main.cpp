#include <iostream>
#include <fstream>
#include <vector>
#include "support.h"
#include "mathmore.h"
#include "commandline.h"

#include "distvec.h"
#include "samplegen.h"
#include "fileofflinereader.h"



void exp_discrete( std::vector<double> &p, double pmin, int n_step, int n_cnt, double degree, double interval );
void exp_file( std::string filename, double pmin, int n_step, int n_cnt, double degree, double interval );

int main(int argc, char *argv[])
{
    double pmin;
    double cL = 0.8, cp = 1;
    std::CommandLine cmd;
    cmd.AddValue ("cL",  "L=cL log k", cL);
    cmd.AddValue ("cp",  "rEnd=cp log k/n", cp);
    cmd.Parse (argc, argv);
    
    std::vector<double> p;
    p = mix(1000000);
    pmin = min_positive_normalized(p);
    exp_discrete( p, pmin, 50000, 10, cL*log(1.0/pmin), cp*log(1.0/pmin) );
    
    // std::string filename = "hamlet.txt";
    // pmin = 1.0/32000;
    // exp_file( filename, pmin, 1000, 32, cL*log(1.0/pmin), cp*log(1.0/pmin) );

    return 0;
}


void exp_discrete( std::vector<double> &p, double pmin, int n_step, int n_cnt, double degree, double interval  )
{
    int truth = cnt_positive(p);
    
    Support support( pmin ); // set pmin
    support.setInterval( interval ); // Approximation interval is [1/k, interval/n ]
    support.setDegree( degree ); // Polynomial degree. Plug-in if N>L

    int trials = 1;
    std::vector< std::vector<int> > plug(n_cnt), poly(n_cnt), TG(n_cnt), CL1(n_cnt), CL2(n_cnt), J1(n_cnt);
    
    SampleGen gen;
    for ( int seed = 0; seed < trials; ++seed )
    {
        gen.reset();
        gen.setSeed( seed );
        for ( int i = 0; i < n_cnt; i++)
        {
            gen.discrete( n_step, p );
            support.setHist( gen.getHist() );
            plug[i].push_back( (int)support.estimate_plug() ); 
            poly[i].push_back( (int)support.estimate() );
            TG[i].push_back( (int)support.estimate_TG() );
            CL1[i].push_back( (int)support.estimate_CL1() );
            CL2[i].push_back( (int)support.estimate_CL2() );
            J1[i].push_back( (int)support.estimate_J1() );
        }
    }

    int n = 0; 
    for ( int i = 0; i < n_cnt; i++)
    {
        n += n_step;
        std::cout<<n<<"\t"<<truth<<"\t"<<mean(plug[i])<<"\t"<<mean(poly[i])<<"\t"<<mean(TG[i])<<"\t"<<mean(CL1[i])<<"\t"<<mean(CL2[i])<<"\t"<<mean(J1[i])<<std::endl;
        // printf("%d\t%d\t  %.1f\t%.6f\t%.6f\t  %.1f\t%.6f\t%.6f\t  %.1f\t%.6f\t%.6f\t  %.1f\t%.6f\t%.6f\t  %.1f\t%.6f\t%.6f\t  \n", 
        //        n, 
        //        truth,

        //        mean(plug[i]), 
        //        stdev(plug[i]), 
        //        rmse(plug[i], truth),

        //        mean(poly[i]),
        //        stdev(poly[i]), 
        //        rmse(poly[i], truth),

        //        mean(TG[i]),
        //        stdev(TG[i]), 
        //        rmse(TG[i], truth),

        //        mean(CL1[i]),
        //        stdev(CL1[i]), 
        //        rmse(CL1[i], truth),

        //        mean(CL2[i]),
        //        stdev(CL2[i]), 
        //        rmse(CL2[i], truth)
        //     );
    }
}




void exp_file( std::string filename, double pmin, int n_step, int n_cnt, double degree, double interval )
{
    FileOfflineReader f( filename );
    int truth = f.distinctTotal();

    Support support( pmin ); // set pmin
    support.setInterval( interval ); // Approximation interval is [1/k, interval/n ]
    support.setDegree( degree ); // Polynomial degree. Plug-in if N>L
	
    int trials = 50;
    std::vector< std::vector<int> > plug(n_cnt), poly(n_cnt), TG(n_cnt), CL1(n_cnt), CL2(n_cnt), J1(n_cnt);
    
    for ( int seed = 0; seed < trials; ++seed )
    {
        f.reset();
        f.setSeed(seed);
        for ( int i = 0; i < n_cnt; i++)
        {
            f.randread( n_step );
            support.setHist( f.getHist() );
            plug[i].push_back( (int)support.estimate_plug() ); 
            poly[i].push_back( (int)support.estimate() );
            TG[i].push_back( (int)support.estimate_TG() );
            CL1[i].push_back( (int)support.estimate_CL1() );
            CL2[i].push_back( (int)support.estimate_CL2() );
            J1[i].push_back( (int)support.estimate_J1() );
        }
    }
    
    int n = 0; 
    for ( int i = 0; i < n_cnt; i++)
    {
        n += n_step;
        // std::cout<<n<<"\t"<<truth<<"\t"<<mean(plug[i])<<"\t"<<mean(poly[i])<<"\t"<<mean(TG[i])<<"\t"<<mean(CL1[i])<<"\t"<<mean(CL2[i])<<"\t"<<mean(J1[i])<<std::endl;
        printf("%d\t%d\t  %.1f\t%.6f\t%.6f\t  %.1f\t%.6f\t%.6f\t  %.1f\t%.6f\t%.6f\t  %.1f\t%.6f\t%.6f\t  %.1f\t%.6f\t%.6f\t  \n", 
               n, 
               truth,

               mean(plug[i]), 
               stdev(plug[i]), 
               rmse(plug[i], truth),

               mean(poly[i]),
               stdev(poly[i]), 
               rmse(poly[i], truth),

               mean(TG[i]),
               stdev(TG[i]), 
               rmse(TG[i], truth),

               mean(CL1[i]),
               stdev(CL1[i]), 
               rmse(CL1[i], truth),

               mean(CL2[i]),
               stdev(CL2[i]), 
               rmse(CL2[i], truth)
            );
    }
}




