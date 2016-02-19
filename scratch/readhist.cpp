#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include "support.h"
#include "mathmore.h"
#include "commandline.h"


#include <dirent.h>


int main(int argc, char *argv[])
{
    double pmin = 1.0/1e6;
    double cL = 0.56, cp = 0.5;
    std::string dirname = "/home/Albus/Dropbox/Work/pengkun/support/1989_hist/";
    
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
        if( pDIR=opendir(dirname.c_str()) )
        {
            while(entry = readdir(pDIR)){
                if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
                    files.push_back( std::string(entry->d_name) );
            }
            closedir(pDIR);
        }
    }

    

    std::map<std::string, int> hist;
    std::cout<<"Samples\tPlug-in\tPolynomial\tGood-Turing\n";
    for( auto name : files )
    {
        std::string filename = dirname+name;
        {
            // std::cout<<filename<<std::endl;
            std::ifstream infile;
            infile.open( filename.c_str() );

            std::string data;
            int cnt;
            while ( (infile >> data).good() )
            {
                infile >> cnt;
                auto it = hist.find( data );
                if ( it == hist.end() )
                    hist.insert( std::make_pair( data ,cnt ) );
                else
                    it->second += cnt;
            }
            infile.close();
        }
        
        std::vector<int> freq;
        for( const auto & pair:hist )
        {
            // std::cout<<pair.second<<std::endl;
            freq.push_back(pair.second);
        }
        
        support.setHist( freq );
        std::cout<<support.getSampleSize()<<"\t"
                 <<(int)support.estimate_plug()<<"\t"
                 <<(int)support.estimate()<<"\t"
                 <<(int)support.estimate_TG()<<"\t"
                 <<std::endl;
    }

    return 0;
}





