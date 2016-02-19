#include <iostream>
#include <fstream>
#include <vector>
#include "support.h"
#include "mathmore.h"
#include "commandline.h"


int main(int argc, char *argv[])
{
    double pmin = 1.0/200;
    double cL = 0.56, cp = 0.5;
    int exp = 0;
    int trials = 50;
    int n_step = 20, n_cnt = 20;
    std::CommandLine cmd;
    cmd.AddValue ("pmin",  "pmin", pmin);
    cmd.AddValue ("cL",  "L=cL log k", cL);
    cmd.AddValue ("cp",  "rEnd=cp log k/n", cp);
    cmd.AddValue ("exp",  "experiment type", exp);
    cmd.AddValue ("trials",  "number of trials", trials);
    cmd.Parse (argc, argv);

    Support support( pmin ); // set pmin
    support.setInterval( cL*log(1.0/pmin) ); // Approximation interval is [1/k, interval/n ]
    support.setDegree( cp*log(1.0/pmin)); // Polynomial degree. Plug-in if N>L


    support.setFin( "fin-spe.txt" );
    std::cout<<support.estimate()+846<<std::endl;

    return 0;
}
