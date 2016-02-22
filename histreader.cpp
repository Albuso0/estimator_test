#include "histreader.h"
#include <fstream>
#include <string>

void HistReader::addHist(std::string filename)
{
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
std::vector<int> HistReader::getHist() const             // return histogram
{
    std::vector<int> freq;
    for( const auto & pair:hist )
        freq.push_back(pair.second);
    return freq;
}

int HistReader::wordN() const                            // the number of words read
{
    int cnt = 0;
    for( const auto & pair:hist )
        cnt += pair.second;
    return cnt;
}
