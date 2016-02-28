#ifndef FILEOFFLINEREADER_H
#define FILEOFFLINEREADER_H

#include "hister.h"
#include <string>
#include <vector>
#include <random>


// FileOfflineReader can read a file can generate histogram of words in the file.
// This is an offline reader, we store the whole file when loading, thus is not suitable for huge files.
class FileOfflineReader
{
public:
    FileOfflineReader(){}
    FileOfflineReader(std::string filename);
    virtual ~FileOfflineReader(){}

    void reset(){ d_pos=0; d_hist.reset(); }
    void setPos( unsigned pos ){ d_pos = pos; }
    void setSeed( int seed ){ d_generator.seed( seed ); }
    
    void readAll();                                   // read the whole file
    void randread( unsigned N );                      // randomly read N words in the file
    void read( unsigned N );                          // read the next N words in the file
    void read( const std::vector<unsigned> &PosVec ); // read words from position vector. Index starts from 0
    

    std::vector<unsigned> getHist() const { return d_hist.getHist(); }
    void printHist() const { d_hist.printHist(); }
    unsigned samplesN() const { return d_hist.samplesN(); } // the number of words that have been read
    size_t distinctN() const { return d_hist.distinctN(); }
    size_t distinctTotal() const;                          // the total number of distinct words over the whole file
    size_t wordTotal() const { return d_words.size(); }      // the total number of words over the whole file
	
private:
    Hister<std::string> d_hist;
    std::vector<std::string> d_words;
    unsigned d_pos;

    std::default_random_engine d_generator;
};



#endif
