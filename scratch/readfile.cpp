#include <iostream>
#include <fstream>
#include <vector>
#include "support.h"
#include "mathmore.h"
#include "commandline.h"

#include "fileofflinereader.h"




int main(int argc, char *argv[])
{
    double pmin = 1.0/32000;
    double cL = 0.56, cp = 0.5;
    int trials = 50;
    int n_step = 1000, n_cnt = 32;
    std::string filename = "hamlet_lower_nopun.txt";
    std::CommandLine cmd;
    cmd.AddValue ("pmin",  "pmin", pmin);
    cmd.AddValue ("cL",  "L=cL log k", cL);
    cmd.AddValue ("cp",  "rEnd=cp log k/n", cp);
    cmd.AddValue ("trials",  "number of trials", trials);
    cmd.Parse (argc, argv);


    FileOfflineReader f( filename );
    int truth = f.distinctTotal();

    Support support( pmin ); // set pmin
    support.setInterval( cL*log(1.0/pmin) ); // Approximation interval is [1/k, interval/n ]
    support.setDegree( cp*log(1.0/pmin) ); // Polynomial degree. Plug-in if N>L
	
    printf("Support size\t\t=%d\n",truth);
    printf("pmin value in estimator\t=%.2e\n", support.getPmin());
    printf("Degree of polynomial\t=%d\n", support.getDegree());
    printf("Approximation interval\t=[%.2e,%.2f/n]\n", support.getPmin(), support.getInterval());
    printf("Sample\tTruth\tPlug-in(mean, stdev, rmse)\tPolynomial(mean, stdev, rmse)\tTuring-Good(mean, stdev, rmse)\tChao-Lee1(mean, stdev, rmse)\tChao-Lee2(mean, stdev, rmse)\n");
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
        printf("%d\t%d\t  %.1f %.2e %.2e\t  %.1f %.2e %.2e\t  %.1f %.2e %.2e\t  %.1f %.2e %.2e\t  %.1f %.2e %.2e\t  \n", 
               n, truth,
               mean(plug[i]), stdev(plug[i]), rmse(plug[i], truth),
               mean(poly[i]), stdev(poly[i]), rmse(poly[i], truth),
               mean(TG[i]), stdev(TG[i]), rmse(TG[i], truth),
               mean(CL1[i]), stdev(CL1[i]), rmse(CL1[i], truth),
               mean(CL2[i]), stdev(CL2[i]), rmse(CL2[i], truth)
            );
    }
    
    return 0;
}





