#include "support.h"
#include "distvec.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "commandline.h"

#include "samplegen.h"
#include "fileofflinereader.h"


// In the following cN is not used in estimator-nosplitting
void exp_discrete( std::vector<double> &p, int pmin_inv, int n_step, int n_cnt, double cL, double cp );
void exp_file( std::string filename, int pmin_inv, int n_step, int n_cnt, double cL, double cp );
double rmse( std::vector<int> v, int truth );
double stdev( std::vector<int> v );
double mean( std::vector<int> v );

int main(int argc, char *argv[])
{
    int k = 1000000;
    double cL = 0.8, cp = 1;
    int exp = 0;
    std::CommandLine cmd;
    cmd.AddValue ("cL",  "L=cL log k", cL);
    cmd.AddValue ("cp",  "rEnd=cp log k/n", cp);
    cmd.AddValue ("exp",  "0=uniform,1=mix,2=hamlet", exp);
    cmd.Parse (argc, argv);
    
    
    std::vector<double> p;
    int pmin_inv;
    switch(exp)
    {
    case 0:
    {
        p = uniform(k);
        pmin_inv = k;
        exp_discrete( p, pmin_inv, 50000, 20, cL, cp );
        break;
    }
    case 1:
        p = mix(k);
        pmin_inv = 2*k;
        exp_discrete( p, pmin_inv, 50000, 20, cL, cp );
        break;
    
    case 2:
    {
        std::string filename = "hamlet.txt";
        pmin_inv = 32000;
        exp_file( filename, pmin_inv, 1000, 32, cL, cp );
    }
    }

    return 0;
}


void exp_discrete( std::vector<double> &p, int pmin_inv, int n_step, int n_cnt, double cL, double cp  )
{
    SampleGen gen;
	
    int truth = p.size();

    Support<int> support( pmin_inv ); // set 1/p_min
    support.setCP(cp); // Approximation interval is [1/k, min{ 1, c_p * log(k) / n }]
    support.setCL(cL); // L = c_L * log(k) and plug-in if N>L

    int trials = 50;
    std::vector< std::vector<int> > plug(n_cnt), poly(n_cnt), TG(n_cnt), CL1(n_cnt), CL2(n_cnt);
    for ( int seed = 0; seed < trials; ++seed )
    {
        gen.reset();
        gen.setSeed( seed );
        for ( int i = 0; i < n_cnt; i++)
        {
            gen.discrete( n_step, &p );
            support.setHist( gen.getHist() );
            plug[i].push_back( (int)support.estimate_plug() ); 
            poly[i].push_back( (int)support.estimate2() );
            TG[i].push_back( (int)support.estimate_TG() );
            CL1[i].push_back( (int)support.estimate_CL1() );
            CL2[i].push_back( (int)support.estimate_CL2() );
        }
    }

    int n = 0; 
    for ( int i = 0; i < n_cnt; i++)
    {
        n += n_step; 
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




void exp_file( std::string filename, int pmin_inv, int n_step, int n_cnt, double cL, double cp )
{
    FileOfflineReader f( filename );
    f.reset();
	
    int truth = f.distinctTotal();

    Support<std::string> support( pmin_inv ); // set 1/p_min
    support.setCP(cp); // Approximation interval is [1/k, min{ 1, c_p * log(k) / n }]
    support.setCL(cL); // L = c_L * log(k) and plug-in if N>L
	
    int n = 0; 
    for ( int i = 0; i < n_cnt; i++)
    {
        n += n_step; 
        std::vector<int> plug, poly;
        for ( int seed = 0; seed < 50; ++seed )
        {
            f.reset();
            srand(seed);
            f.randread( n );
            support.setHist( f.getHist() );
            plug.push_back( (int)support.estimate_plug() ); 
            poly.push_back( (int)support.estimate2() );
        }
        printf("%d\t%d\t  %d\t%d\t%d\t%.6f\t%.6f\t  %d\t%d\t%d\t%.6f\t%.6f\t  \n", 
               n, 
               truth,

               std::accumulate(plug.begin(), plug.end(), 0) / 50, 
               *min_element(plug.begin(), plug.end()),
               *max_element(plug.begin(), plug.end()),
               stdev(plug),
               rmse(plug, truth),

               std::accumulate(poly.begin(), poly.end(), 0) / 50, 
               *min_element(poly.begin(), poly.end()),
               *max_element(poly.begin(), poly.end()),
               stdev(poly),
               rmse(poly, truth)
            );		
    }
}




double mean( std::vector<int> v)
{
    return (std::accumulate(v.begin(), v.end(), 0.0) * 1.0 / v.size()); 
}

double rmse( std::vector<int> v, int truth )
{
    double sum = 0;
    for ( const auto &x : v )
        sum += 1.0*(x-truth)*(x-truth);
    return sqrt( sum / v.size() );
}

double stdev( std::vector<int> v )
{
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    double mean = sum / v.size();

    std::vector<double> diff(v.size());
    std::transform(v.begin(), v.end(), diff.begin(), std::bind2nd(std::minus<double>(), mean));
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    return std::sqrt(sq_sum / (v.size()-1) );
}
