#include "fileofflinereader.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <set>
#include "stringop.h"

std::string FileOfflineReader::stdString(std::string word) const
{
    // StringOp::removepunc(word);
    // StringOp::tolower(word);
    return word;
}


void FileOfflineReader::addWord(std::string word)
{
    auto it = hist.find( word );
    if ( it == hist.end() )
        hist.insert( std::pair<std::string,int>( word,1 ) );
    else
        ++(it->second);
}


/* --------------- IMPLEMENTATIAON OF READ ----------------*/
void FileOfflineReader::randread(int N)
{
    for ( int i = 0; i < N;  ++i )
    {
        int loc = rand() % words.size();
        addWord(stdString(words[loc]));
    }
}

void FileOfflineReader::read(int N)
{
    for ( int i = 0; i < N;  ++i )
    {
        if ( pos < words.size() )
        {
            addWord(stdString(words[pos]));
            ++pos;
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
        if ( index < words.size() )
            addWord(stdString(words[index]));
}
/* --------------- END IMPLEMENTATIAON OF READ ----------------*/



int FileOfflineReader::wordN() const
{
    int sum = 0;
    for ( const auto & pair : hist )
        sum += pair.second;
    return sum;	
}


FileOfflineReader::FileOfflineReader(std::string filename)
    :FileOfflineReader()
{
    std::ifstream infile;
    infile.open( filename.c_str() );

    std::string str_in;
    while ( (infile >> str_in).good() )
        words.push_back(str_in);
	
    infile.close();
}

int FileOfflineReader::distinctTotal() const
{
    std::set<std::string> wordset;
    for ( const auto & x:words )
        wordset.insert( stdString(x) );
    return wordset.size();
}

std::vector<int> FileOfflineReader::getHist() const
{
    std::vector<int> out_hist;
    for ( const auto & pair : hist )
        out_hist.push_back( pair.second );
    return out_hist;
}
