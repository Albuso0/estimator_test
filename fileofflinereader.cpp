#include "fileofflinereader.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <set>



/* --------------- IMPLEMENTATIAON OF READ ----------------*/
void FileOfflineReader::readAll()
{
    for ( const auto & word:words)
        addSample(word);
}
void FileOfflineReader::randread(unsigned long long N)
{
    std::uniform_int_distribution<int> uniform(0,words.size()-1);
    for ( unsigned long long i = 0; i < N;  ++i )
        addSample(words[uniform(generator)]);
}

void FileOfflineReader::read(unsigned long long N)
{
    for ( unsigned long long i = 0; i < N;  ++i )
    {
        if ( pos < words.size() )
        {
            addSample(words[pos]);
            ++pos;
        }
        else
        {
            break;
        }
    }
}

void FileOfflineReader::read( const std::vector<unsigned long long> &PosVec )
{
    for ( const auto & index:PosVec )
        if ( index < words.size() )
            addSample(words[index]);
}
/* --------------- END IMPLEMENTATIAON OF READ ----------------*/




FileOfflineReader::FileOfflineReader(std::string filename)
    :FileOfflineReader()
{
    std::ifstream infile;
    infile.open( filename.c_str() );

    std::string str_in;
    while ( infile >> str_in )
        words.push_back(str_in);
    
    infile.close();
}

unsigned long long FileOfflineReader::distinctTotal() const
{
    std::set<std::string> wordset;
    for ( const auto & x:words )
        wordset.insert( x );
    return wordset.size();
}

