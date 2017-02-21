#include <iostream>
#include <fstream>
#include <vector>
#include "support.h"
#include "mathmore.h"
#include "commandline.h"

#include "distvec.h"
#include "samplegen.h"




int main(int argc, char *argv[])
{
    double pmin = 1.0/1e6;
    int exp = 0;
    int trials = 50;
    unsigned n_step = 50000, n_cnt = 20;
    std::CommandLine cmd;
    cmd.AddValue ("pmin",  "pmin", pmin);
    cmd.AddValue ("exp",  "experiment type", exp);
    cmd.AddValue ("trials",  "number of trials", trials);
    cmd.Parse (argc, argv);

    Support support( pmin ); // set pmin
    // support.setDegree( 0.45*log(1.0/pmin)); // Polynomial degree. Plug-in if N>L
    // support.setInterval( 0.5*log(1.0/pmin) ); // Approximation interval is [1/k, interval/n ]

    
    std::vector<double> p;

    switch(exp)
    {
    case 0:
        p = uniform(1000000);
        break;
    case 1:
        p = mix(500000);
        break;
    case 2:
        p = zipf(83928);
        break;
    case 3:
        p = zipfd5(500516);
        break;
    case 4:
        p = mixgeozipf(88677);
        break;
    default:
        break;
    }

    // switch(exp)
    // {
    // case 0:
    //     p = uniform(200);
    //     break;
    // case 1:
    //     p = mix(100);
    //     break;
    // case 2:
    //     p = zipf(45);
    //     break;
    // case 3:
    //     p = zipfd5(107);
    //     break;
    // case 4:
    //     p = mixgeozipf(51);
    //     break;
    // default:
    //     break;
    // }
    
    
    // pmin = min_positive_normalized(p);
    // std::cout<<pmin<<std::endl;
    int truth = cnt_positive(p);
    printf("Support size\t\t=%d\n",truth);
    printf("Minimum non-zero mass\t=%.2e\n",min_positive_normalized(p));
    printf("pmin value in estimator\t=%.2e\n", support.getPmin());
    printf("Degree of polynomial\t=%d\n", support.getDegree());
    printf("Approximation interval\t=[%.2e,%.2f/n]\n", support.getPmin(), support.getInterval());
    printf("Number of trials\t=%d\n", trials);
    printf("Sample\tTruth\tPlug-in(mean, stdev, rmse)\tPolynomial(mean, stdev, rmse)\tTuring-Good(mean, stdev, rmse)\tChao-Lee1(mean, stdev, rmse)\tChao-Lee2(mean, stdev, rmse)\tChao1(mean, stdev, rmse)\n");

    std::vector< std::vector<int> > plug(n_cnt), poly(n_cnt), TG(n_cnt), CL1(n_cnt), CL2(n_cnt), J1(n_cnt), Chao1(n_cnt);
    SampleGen gen;
    for ( int seed = 1; seed <= trials; ++seed )
    {
        gen.resetHist();
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
            Chao1[i].push_back( (int)support.estimate_Chao1() );
        }
    }

    int n = 0; 
    for ( int i = 0; i < n_cnt; i++)
    {
        n += n_step;
        printf("%d\t%d\t  %.1f %.2e %.2e\t  %.1f %.2e %.2e\t  %.1f %.2e %.2e\t  %.1f %.2e %.2e\t  %.1f %.2e %.2e\t  %.1f %.2e %.2e\t  \n", 
               n, truth,
               mean(plug[i]), stdev(plug[i]), rmse(plug[i], truth),
               mean(poly[i]), stdev(poly[i]), rmse(poly[i], truth),
               mean(TG[i]), stdev(TG[i]), rmse(TG[i], truth),
               mean(CL1[i]), stdev(CL1[i]), rmse(CL1[i], truth),
               mean(CL2[i]), stdev(CL2[i]), rmse(CL2[i], truth),
               mean(Chao1[i]), stdev(Chao1[i]), rmse(Chao1[i], truth)
            );
    }



    return 0;
}




