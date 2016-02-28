#ifndef FILEOFFLINELINEREADER_H
#define FILEOFFLINELINEREADER_H

#include "hister.h"
#include <string>
#include <random>


class FileOfflineLineReader
{
public:
    FileOfflineLineReader(){}
    FileOfflineLineReader(std::string filename);
    virtual ~FileOfflineLineReader(){}

    void reset(){ d_hist.reset(); }
    void setSeed( int seed ){ d_generator.seed( seed ); }
    void readAll();                                             // read the whole file
    void randread( unsigned N );                                // randomly read N lines in the file

    
    std::vector<unsigned> getHist() const { return d_hist.getHist(); }
    void printHist() const { d_hist.printHist(); }
    unsigned samplesN() const { return d_hist.samplesN(); } // the number of words that have been read
    size_t distinctN() const { return d_hist.distinctN(); }
    size_t linesTotal() const { return d_lines.size(); }
    size_t distinctTotal() const;
private:
    Hister<std::string> d_hist;
    std::vector<std::string> d_lines;
    std::default_random_engine d_generator;

    void addLine(const std::string &line);
};



#endif
