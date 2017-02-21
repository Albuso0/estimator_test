#include "finop.h"
#include <iostream>
#include <fstream>
#include <map>

void addFin( vfin &fin, int freq, int cnt)
{
    fin.push_back( std::make_pair( freq, cnt ) );
}

void setFin( vfin &fin, const std::string filename) 
{
    fin.clear();
    std::ifstream infile;
    infile.open( filename.c_str() );
    int freq, cnt;
    while ( (infile>>freq).good() )
    {
        infile >> cnt;
        addFin( fin, freq, cnt );
    }
    infile.close();
}

void setFin( vfin &fin, const vint &vfreq, const vint &vcnt)
{
    fin.clear();
    for ( int i = 0; i < vfreq.size(); i++)
    {
        int freq = vfreq[i], cnt = vcnt[i];
        addFin( fin, freq, cnt );
    }
}

void setHist( vfin &fin, const vint &hist )
{
    fin.clear();
    
    std::map<int, int> fin_map;
    for ( const auto & freq : hist )
    {
        auto iter = fin_map.find( freq );
        if ( iter == fin_map.end() )
            fin_map.insert( std::make_pair( freq,1 ) );
        else
            ++(iter->second);
    }
    
    for ( auto it = fin_map.begin(); it != fin_map.end(); ++it )
    {
        int freq = it->first, cnt = it->second;
        addFin( fin, freq, cnt );
    }
}

void setHist( vfin &fin, const std::string filename) 
{
    // Read histogram from file
    vint hist;
    std::ifstream infile;
    infile.open( filename.c_str() );
    int freq;
    while ( (infile>>freq).good() )
    {
        if ( freq > 0)
        {
            hist.push_back(freq);
        }
    }
    infile.close();
    // set fingerprint from histogram
    setHist(fin,hist);
}


int getSampleSize( const vfin &fin ) 
{
    int n = 0;
    for ( auto it = fin.begin(); it != fin.end(); ++it )
    {
        int freq = it->first, cnt = it->second;
        n += (freq * cnt);
    }
    return n;
}
