#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include "filereader.h"


/* -------------------- READ FILE -----------------------*/
void FileReader::read(unsigned long long N)
{
    std::string word;
    for ( unsigned long long i = 0; i < N;  ++i )
    {
        if ( infile >> word )
            addSample(word);
        else
            break;
    }
}
/* -------------------- END READ FILE -----------------------*/



unsigned long long FileReader::distinctTotal()
{
    saveState();
    resetState();
    std::istream_iterator<std::string> in(infile), end;
    std::set<std::string> words;
    for (; in != end; ++in)
    {
        words.insert(*in);
    }
    loadState();
    return words.size();
}

unsigned long long FileReader::wordTotal()
{
    saveState();
    resetState();
    std::istream_iterator<std::string> in(infile), end;
    int count = std::distance(in, end);
    loadState();
	
    return count;
}

void FileReader::saveState()
{
    fState = infile.rdstate();
    fPos = infile.tellg();
}
void FileReader::resetState()
{
    infile.clear();
    infile.seekg(0);
}
void FileReader::loadState()
{
    infile.clear( (std::ios_base::iostate)fState );
    infile.seekg(fPos);
}


FileReader::FileReader(std::string filename)
    :FileReader()
{
    infile.open( filename.c_str() );
}

FileReader::~FileReader()
{
    infile.close();
}

