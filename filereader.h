#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <fstream>
#include <map>
#include <memory>
#include <vector>

// FileReader can read a file can generate histogram of words in the file.
// This is an online reader, we do not require memory to store all huge file.
class FileReader
{
public:
    FileReader(){}
    FileReader(std::string filename);
    virtual ~FileReader();

    void reset(){ resetHist(); resetFile(); }
    
    void read(unsigned long long N); // read the next N words in the file, return the pointer to updated histogram.

    std::vector<int> getHist() const;             // return histogram

    unsigned long long wordN(); // the number of words that have been read
    unsigned long long distinctN() { return hist.size(); }

    unsigned long long distinctTotal(); // total number of distinct words
    unsigned long long wordTotal(); // total number of words
protected:
    void resetHist(){ hist.clear(); }
    void resetFile(){ resetState(); }
	
private:
    std::ifstream infile;
    std::map<std::string, int> hist;

    void addWord(std::string word);

    // process the state inside the ifstream mpFile
    void saveState();
    void resetState();
    void loadState();
    int fState, fPos;
};



#endif
