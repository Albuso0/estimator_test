#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include "filereader.h"


/* -------------------- READ FILE -----------------------*/
void FileReader::read(unsigned N)
{
    std::string word;
    for ( unsigned i = 0; i < N;  ++i )
    {
        if ( d_infile >> word )
            d_hist.addSample(word);
        else
            break;
    }
}
/* -------------------- END READ FILE -----------------------*/



unsigned FileReader::distinctTotal()
{
    saveFileState();
    resetFileState();
    std::istream_iterator<std::string> in(d_infile), end;
    std::set<std::string> words;
    for (; in != end; ++in)
    {
        words.insert(*in);
    }
    loadFileState();
    return words.size();
}

unsigned FileReader::wordTotal()
{
    saveFileState();
    resetFileState();
    std::istream_iterator<std::string> in(d_infile), end;
    unsigned count = std::distance(in, end);
    loadFileState();
	
    return count;
}

void FileReader::saveFileState()
{
    d_fState = d_infile.rdstate();
    d_fPos = d_infile.tellg();
}
void FileReader::resetFileState()
{
    d_infile.clear();
    d_infile.seekg(0);
}
void FileReader::loadFileState()
{
    d_infile.clear( (std::ios_base::iostate)d_fState );
    d_infile.seekg(d_fPos);
}


FileReader::FileReader(std::string filename)
    :FileReader()
{
    d_infile.open( filename.c_str() );
}

FileReader::~FileReader()
{
    d_infile.close();
}

