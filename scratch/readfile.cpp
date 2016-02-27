#include <iostream>
#include <fstream>
#include <vector>
#include "support.h"
#include "commandline.h"
#include "mathmore.h"

#include "fileofflinereader.h"
#include "fileofflinelinereader.h"




int main(int argc, char *argv[])
{
    double pmin = 1.0/50e6;
    int trials = 50;
    unsigned n_step = 20000, n_cnt = 30;
    std::string filename = "/home/Albus/Data_disk/NYT/1989_whole_paragraphs.txt";
    // std::string filename = "../hamlet.txt";
    
    std::CommandLine cmd;
    cmd.AddValue ("pmin",  "pmin", pmin);
    cmd.AddValue ("trials",  "number of trials", trials);
    cmd.Parse (argc, argv);


    // FileOfflineReader f( filename );
    FileOfflineLineReader f( filename );
    // std::cout<<f.distinctTotal()<<"\n";
    // std::cout<<f.linesTotal()<<"\n";
    
    Support support( pmin ); // set pmin

    // f.reset();
    // f.setSeed(0);
    // printf("pmin value in estimator\t=%.2e\n", support.getPmin());
    // printf("Degree of polynomial\t=%d\n", support.getDegree());
    // printf("Approximation interval\t=[%.2e,%.2f/n]\n", support.getPmin(), support.getInterval());
    // std::cout<<"Samples\tPlug-in\tPoly\tGood-Turing\n";
    // for ( unsigned long i = 0; i < n_cnt; i++)
    // {
    //     // f.readAll();
    //     // std::cout<<std::endl;
    //     // f.printHist();
    //     f.randread(n_step);
    //     support.setHist( f.getHist() );
    //     std::cout<<support.getSampleSize()<<"\t"
    //              <<(int)support.estimate_plug()<<"\t"
    //              <<(int)support.estimate()<<"\t"
    //              <<(int)support.estimate_TG()<<"\t"
    //              <<std::endl;
    // }




    



    
    
    int truth = f.distinctTotal();
    printf("Support size\t\t=%d\n",truth);
    printf("pmin value in estimator\t=%.2e\n", support.getPmin());
    printf("Degree of polynomial\t=%d\n", support.getDegree());
    printf("Approximation interval\t=[%.2e,%.2f/n]\n", support.getPmin(), support.getInterval());
    printf("Sample\tTruth\tPlug-in(mean, stdev, rmse)\tPolynomial(mean, stdev, rmse)\tTuring-Good(mean, stdev, rmse)\tChao-Lee1(mean, stdev, rmse)\tChao-Lee2(mean, stdev, rmse)\n");
    std::vector< std::vector<int> > plug(n_cnt), poly(n_cnt), TG(n_cnt), CL1(n_cnt), CL2(n_cnt), J1(n_cnt);
    for ( int seed = 1; seed <= trials; ++seed )
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




    
    // std::vector< std::pair<int, int> > fin = support.getFin();
    // for (int i = 0; i<8; ++i)
    // {
    //     int freq = fin[i].first, cnt = fin[i].second;
    //     double gL = support.getCoeff(freq);
    //     std::cout<<freq<<" "<<cnt<<" "<<gL<<" "<<1.0*cnt*gL<<" "<<"\n";
    // }
    // int cnt = 0;
    // for (unsigned i = 8; i<fin.size(); ++i)
    //     cnt += fin[i].second;
    // std::cout<<cnt<<"\n";

    return 0;
}





