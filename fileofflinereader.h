#ifndef FILEOFFLINEREADER_H
#define FILEOFFLINEREADER_H

#include "hister.h"
#include <string>
#include <vector>
#include <random>


// FileOfflineReader can read a file can generate histogram of words in the file.
// This is an offline reader, we store the whole file when loading, thus is not suitable for huge files.
class FileOfflineReader:
    public Hister<std::string>
{
public:
    FileOfflineReader(){}
    FileOfflineReader(std::string filename);
    virtual ~FileOfflineReader(){}

    void reset(){ resetPos(); resetHist(); }
    void setPos( int _pos ){ pos = _pos; }
    void setSeed( int seed ){ generator.seed( seed ); }
    
    void readAll();                                             // read the whole file
    void randread( unsigned long long N );                      // randomly read N words in the file
    void read( unsigned long long N );                          // read the next N words in the file
    void read( const std::vector<unsigned long long> &PosVec ); // read words from position vector. Index starts from 0
    

    unsigned long long distinctTotal() const;                          // the total number of distinct words over the whole file
    unsigned long long wordTotal() const { return words.size(); }      // the total number of words over the whole file
protected:
    void resetPos() { pos = 0; }
    void resetHist(){ hist.clear(); }
	
private:
    std::vector<std::string> words;
    unsigned pos;

    std::default_random_engine generator;
};



#endif
