#include "fileofflinelinereader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <random>

void FileOfflineLineReader::addLine(const std::string &line)
{
    std::istringstream iss(line);
    std::string word;
    while ( iss >> word )
        addSample(word);
}

/* --------------- IMPLEMENTATIAON OF READ ----------------*/
void FileOfflineLineReader::readAll()
{
    for ( const auto & line:lines)
        addLine(line);
}
void FileOfflineLineReader::randread(unsigned long N)
{
    std::uniform_int_distribution<int> uniform(0,lines.size()-1);
    for (unsigned long i = 0; i < N; i++)
        addLine(lines[uniform(generator)]);
}
/* --------------- END IMPLEMENTATIAON OF READ ----------------*/



FileOfflineLineReader::FileOfflineLineReader(std::string filename)
    :FileOfflineLineReader()
{
    std::ifstream infile;
    infile.open( filename.c_str() );

    std::string line;
    while ( std::getline(infile, line) )
        lines.push_back(line);
    
    infile.close();
    
}


unsigned long FileOfflineLineReader::distinctTotal() const
{
    std::set<std::string> wordset;
    for ( const auto &line:lines )
    {
        std::istringstream iss(line);
        std::string word;
        while ( iss >> word )
            wordset.insert( word );
    }
    return wordset.size();
}
