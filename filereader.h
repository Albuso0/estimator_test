#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <fstream>
#include <vector>
#include "hister.h"

// FileReader can read a file can generate histogram of words in the file.
// This is an online reader, we do not require memory to store all huge file.
class FileReader
{
public:
    FileReader(){}
    FileReader(std::string filename);
    virtual ~FileReader();

    void reset(){ d_hist.reset(); resetFileState(); }
    void read(unsigned N); // read the next N words in the file, return the pointer to updated histogram.

    
    std::vector<unsigned> getHist() const { return d_hist.getHist(); }
    void printHist() const { d_hist.printHist(); }
    unsigned samplesN() const { return d_hist.samplesN(); } // the number of words that have been read
    size_t distinctN() const { return d_hist.distinctN(); }
    unsigned distinctTotal(); // total number of distinct words
    unsigned wordTotal(); // total number of words
    
private:
    Hister<std::string> d_hist;
    std::ifstream d_infile;

    // process the state inside the ifstream mpFile
    void saveFileState();
    void resetFileState();
    void loadFileState();
    int d_fState, d_fPos;
};



#endif
