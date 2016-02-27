#ifndef FILEOFFLINELINEREADER_H
#define FILEOFFLINELINEREADER_H

#include "hister.h"
#include <string>
#include <random>


class FileOfflineLineReader:
    public Hister<std::string>
{
public:
    FileOfflineLineReader(){}
    FileOfflineLineReader(std::string filename);
    virtual ~FileOfflineLineReader(){}

    void reset(){ resetHist(); }
    void setSeed( int seed ){ generator.seed( seed ); }

    size_t linesTotal() const { return lines.size(); }
    unsigned long distinctTotal() const;
    
    void readAll();                                             // read the whole file
    void randread( unsigned long N );                      // randomly read N lines in the file
    /* Inherit from Hister<std::string>:
       public:
       std::vector<int> getHist() const;             // return histogram

       std::size_t samplesN() const; // the number of words that have been read
       std::size_t distinctN() const;
       
       protected:
       void resetHist(){ hist.clear(); }
       
       private:
       std::map<std::string, int> hist;
    */
    
private:
    std::vector<std::string> lines;
    void addLine(const std::string &line);
    
    std::default_random_engine generator;
};



#endif
