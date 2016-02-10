#include <iostream>
#include <fstream>
#include <vector>
#include "support.h"
#include "mathmore.h"
#include "commandline.h"

#include "distvec.h"
#include "samplegen.h"
#include "fileofflinereader.h"



void exp_discrete( std::vector<double> &p, double pmin, int n_step, int n_cnt, double degree, double interval, int trials );
void exp_file( std::string filename, double pmin, int n_step, int n_cnt, double degree, double interval, int trials );

int main(int argc, char *argv[])
{
    double pmin;
    double cL = 0.8, cp = 1;
    int exp = 0;
    int trials = 50;
    std::CommandLine cmd;
    cmd.AddValue ("cL",  "L=cL log k", cL);
    cmd.AddValue ("cp",  "rEnd=cp log k/n", cp);
    cmd.AddValue ("exp",  "experiment type", exp);
    cmd.AddValue ("trials",  "number of trials", trials);
    cmd.Parse (argc, argv);
    
    std::vector<double> p;
    int k = 1000000;
    switch(exp)
    {
    case 0:
        p = uniform(k);
        pmin = min_positive_normalized(p);
        exp_discrete( p, pmin, 50000, 20, cL*log(1.0/pmin), cp*log(1.0/pmin), trials );
        break;
    case 1:
        p = mix(k);
        pmin = min_positive_normalized(p);
        exp_discrete( p, pmin, 50000, 20, cL*log(1.0/pmin), cp*log(1.0/pmin), trials );
        break;
    case 2:
        p = zipf(k);
        pmin = min_positive_normalized(p);
        exp_discrete( p, pmin, 50000, 20, cL*log(1.0/pmin), cp*log(1.0/pmin), trials );
        break;
    case 3:
        p = zipfd5(k);
        pmin = min_positive_normalized(p);
        exp_discrete( p, pmin, 50000, 20, cL*log(1.0/pmin), cp*log(1.0/pmin), trials );
        break;
    case 4:
        p = mixgeozipf(1000000);
        pmin = min_positive_normalized(p);
        exp_discrete( p, pmin, 50000, 20, cL*log(1.0/pmin), cp*log(1.0/pmin), trials );
        break;
    case 5:
        pmin = 1.0/32000;
        exp_file( "hamlet.txt", pmin, 1000, 32, cL*log(1.0/pmin), cp*log(1.0/pmin), trials );
        break;
    default:
        break;
    }
    

    return 0;
}


void exp_discrete( std::vector<double> &p, double pmin, int n_step, int n_cnt, double degree, double interval, int trials  )
{
    int truth = cnt_positive(p);
    
    Support support( pmin ); // set pmin
    support.setInterval( interval ); // Approximation interval is [1/k, interval/n ]
    support.setDegree( degree ); // Polynomial degree. Plug-in if N>L

    printf("Support size\t\t=%d\n",truth);
    printf("Minimum non-zero mass\t=%.2e\n",pmin);
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

    printf("Sample\tTruth\tPlug-in(mean, stdev, rmse)\tPolynomial(mean, stdev, rmse)\tTuring-Good(mean, stdev, rmse)\tChao-Lee1(mean, stdev, rmse)\tChao-Lee2(mean, stdev, rmse)\n");
    int n = 0; 
    for ( int i = 0; i < n_cnt; i++)
    {
        n += n_step;
        printf("%d\t%d\t  %.1f %.2e %.2e\t  %.1f %.2e %.2e\t  %.1f %.2e %.2e\t  %.1f %.2e %.2e\t  %.1f %.2e %.2e\t  \n", 
               n, truth,
               mean(plug[i]), stdev(plug[i]), rmse(plug[i], truth),
               mean(poly[i]), stdev(poly[i]), rmse(poly[i], truth),
               mean(TG[i]), stdev(TG[i]), rmse(TG[i], truth),
               mean(CL1[i]), stdev(CL1[i]), rmse(CL1[i], truth),
               mean(CL2[i]), stdev(CL2[i]), rmse(CL2[i], truth)
            );
    }
}




void exp_file( std::string filename, double pmin, int n_step, int n_cnt, double degree, double interval, int trials )
{
    FileOfflineReader f( filename );
    int truth = f.distinctTotal();

    Support support( pmin ); // set pmin
    support.setInterval( interval ); // Approximation interval is [1/k, interval/n ]
    support.setDegree( degree ); // Polynomial degree. Plug-in if N>L
	
    printf("Support size\t\t=%d\n",truth);
    printf("Minimum non-zero mass\t=%.2e\n",pmin);
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
    
    printf("Sample\tTruth\tPlug-in(mean, stdev, rmse)\tPolynomial(mean, stdev, rmse)\tTuring-Good(mean, stdev, rmse)\tChao-Lee1(mean, stdev, rmse)\tChao-Lee2(mean, stdev, rmse)\n");
    int n = 0; 
    for ( int i = 0; i < n_cnt; i++)
    {
        n += n_step;
        printf("%d\t%d\t  %.1f %.2e %.2e\t  %.1f %.2e %.2e\t  %.1f %.2e %.2e\t  %.1f %.2e %.2e\t  %.1f %.2e %.2e\t  \n", 
               n, truth,
               mean(plug[i]), stdev(plug[i]), rmse(plug[i], truth),
               mean(poly[i]), stdev(poly[i]), rmse(poly[i], truth),
               mean(TG[i]), stdev(TG[i]), rmse(TG[i], truth),
               mean(CL1[i]), stdev(CL1[i]), rmse(CL1[i], truth),
               mean(CL2[i]), stdev(CL2[i]), rmse(CL2[i], truth)
            );
    }
}




