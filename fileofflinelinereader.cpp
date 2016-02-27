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
        d_hist.addSample(word);
}

/* --------------- IMPLEMENTATIAON OF READ ----------------*/
void FileOfflineLineReader::readAll()
{
    for ( const auto & line:d_lines)
        addLine(line);
}
void FileOfflineLineReader::randread(unsigned N)
{
    std::uniform_int_distribution<int> uniform(0, d_lines.size()-1);
    for (unsigned i = 0; i < N; i++)
        addLine(d_lines[uniform(d_generator)]);
}
/* --------------- END IMPLEMENTATIAON OF READ ----------------*/



FileOfflineLineReader::FileOfflineLineReader(std::string filename)
    :FileOfflineLineReader()
{
    std::ifstream infile;
    infile.open( filename.c_str() );

    std::string line;
    while ( std::getline(infile, line) )
        d_lines.push_back(line);
    
    infile.close();
    
}


size_t FileOfflineLineReader::distinctTotal() const
{
    std::set<std::string> wordset;
    for ( const auto &line:d_lines )
    {
        std::istringstream iss(line);
        std::string word;
        while ( iss >> word )
            wordset.insert( word );
    }
    return wordset.size();
}
