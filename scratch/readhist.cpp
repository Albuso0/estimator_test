#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include "support.h"
#include "mathmore.h"
#include "commandline.h"
#include "hister.h"
#include <cstring>

#include <dirent.h>
#include <random>

int main(int argc, char *argv[])
{
    double pmin = 1.0/1e5;
    double cL = 0.56, cp = 0.5;
    std::string dirname = "/home/Albus/Documents/NYT/1989_hist_articles/";
    
    std::CommandLine cmd;
    cmd.AddValue ("pmin",  "pmin", pmin);
    cmd.AddValue ("cL",  "L=cL log k", cL);
    cmd.AddValue ("cp",  "rEnd=cp log k/n", cp);
    cmd.Parse (argc, argv);


    Support support( pmin ); // set pmin
    support.setInterval( cL*log(1.0/pmin) ); // Approximation interval is [1/k, interval/n ]
    support.setDegree( cp*log(1.0/pmin) ); // Polynomial degree. Plug-in if N>L

    
    std::vector<std::string> files;
    {
        DIR *pDIR;
        struct dirent *entry;
        if( (pDIR=opendir(dirname.c_str())) )
        {
            while( (entry = readdir(pDIR)) ){
                if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
                    files.push_back( std::string(entry->d_name) );
            }
            closedir(pDIR);
        }
    }

    int seed = 0;
    int n_cnt = 20, n_step = 4000;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,files.size()-1);
    generator.seed( seed );

    
	
    Hister<std::string> hist;
    printf("pmin value in estimator\t=%.2e\n", support.getPmin());
    printf("Degree of polynomial\t=%d\n", support.getDegree());
    printf("Approximation interval\t=[%.2e,%.2f/n]\n", support.getPmin(), support.getInterval());
    std::cout<<"Samples\tPlug-in\tPoly\tGood-Turing\n";
    for( int i = 0; i < n_cnt; i++)
    {
        for (int j = 0; j < n_step; j++)
        {
            auto name = files[ distribution(generator) ];
            // auto name = files[ n_step*i + j ];
            hist.addHistFile(dirname+name);
        }
        
        support.setHist( hist.getHist() );
        std::cout<<support.getSampleSize()<<"\t"
                 <<(int)support.estimate_plug()<<"\t"
                 <<(int)support.estimate()<<"\t"
                 <<(int)support.estimate_TG()<<"\t"
                 <<std::endl;
    }

    
    std::vector< std::pair<int, int> > fin = support.getFin();
    int cutoff = support.getDegree();
    for (int i = 0; i<cutoff; ++i)
    {
        int freq = fin[i].first, cnt = fin[i].second;
        double gL = support.getCoeff(freq);
        std::cout<<freq<<" "<<cnt<<" "<<gL<<" "<<1.0*cnt*gL<<" "<<"\n";
    }
    int cnt = 0;
    for (size_t i = cutoff; i<fin.size(); ++i)
        cnt += fin[i].second;
    std::cout<<cnt<<"\n";

    return 0;
}





