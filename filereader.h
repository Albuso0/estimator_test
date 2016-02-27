#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <fstream>
#include <vector>
#include "hister.h"

// FileReader can read a file can generate histogram of words in the file.
// This is an online reader, we do not require memory to store all huge file.
class FileReader:
    public Hister<std::string>
{
public:
    FileReader(){}
    FileReader(std::string filename);
    virtual ~FileReader();

    void reset(){ resetHist(); resetFile(); }
    
    void read(unsigned long long N); // read the next N words in the file, return the pointer to updated histogram.

    unsigned long long distinctTotal(); // total number of distinct words
    unsigned long long wordTotal(); // total number of words
protected:
    void resetHist(){ hist.clear(); }
    void resetFile(){ resetState(); }
	
private:
    std::ifstream infile;

    // process the state inside the ifstream mpFile
    void saveState();
    void resetState();
    void loadState();
    int fState, fPos;
};



#endif
