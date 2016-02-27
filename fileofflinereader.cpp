#include "fileofflinereader.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <set>



/* --------------- IMPLEMENTATIAON OF READ ----------------*/
void FileOfflineReader::readAll()
{
    for ( const auto & word:d_words)
        d_hist.addSample(word);
}
void FileOfflineReader::randread(unsigned N)
{
    std::uniform_int_distribution<int> uniform(0,d_words.size()-1);
    for ( unsigned i = 0; i < N;  ++i )
        d_hist.addSample(d_words[uniform(d_generator)]);
}

void FileOfflineReader::read(unsigned N)
{
    for ( unsigned i = 0; i < N;  ++i )
    {
        if ( d_pos < d_words.size() )
        {
            d_hist.addSample(d_words[d_pos]);
            ++d_pos;
        }
        else
        {
            break;
        }
    }
}

void FileOfflineReader::read( const std::vector<unsigned> &PosVec )
{
    for ( const auto & index:PosVec )
        if ( index < d_words.size() )
            d_hist.addSample(d_words[index]);
}
/* --------------- END IMPLEMENTATIAON OF READ ----------------*/




FileOfflineReader::FileOfflineReader(std::string filename)
    :FileOfflineReader()
{
    std::ifstream infile;
    infile.open( filename.c_str() );

    std::string str_in;
    while ( infile >> str_in )
        d_words.push_back(str_in);
    
    infile.close();
}

size_t FileOfflineReader::distinctTotal() const
{
    std::set<std::string> wordset;
    for ( const auto & x:d_words )
        wordset.insert( x );
    return wordset.size();
}

