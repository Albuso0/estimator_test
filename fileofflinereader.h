#ifndef FILEOFFLINEREADER_H
#define FILEOFFLINEREADER_H

#include <string>
#include <map>
#include <memory>
#include <vector>


// FileOfflineReader can read a file can generate histogram of words in the file.
// This is an offline reader, we store the whole file when loading, thus is not suitable for huge files.
class FileOfflineReader
{
public:
    FileOfflineReader(){}
    FileOfflineReader(std::string filename);
    virtual ~FileOfflineReader(){}

    void reset(){ resetPos(); resetHist(); }
    void setPos( int _pos ){ pos = _pos; }
    void setSeed( int seed ){ srand(seed); }
    
    void randread( int N );                      // randomly read N words in the file
    void read( int N );                          // read the next N words in the file
    void read( const std::vector<unsigned> &PosVec ); // read words from position vector. Index starts from 0
    
    std::vector<int> getHist() const;             // return histogram
    int wordN() const;                            // the number of words read
    int distinctN() const { return hist.size(); } // the number of distinct words encountered

    int distinctTotal() const;                          // the total number of distinct words over the whole file
    int wordTotal() const { return words.size(); }      // the total number of words over the whole file
protected:
    void resetPos() { pos = 0; }
    void resetHist(){ hist.clear(); }
	
private:
    std::map<std::string, int> hist;
    
    std::vector<std::string> words;
    unsigned pos;
    
    void addWord(std::string word);
    std::string stdString(std::string word) const; // standarderize the input word 
	
};



#endif
