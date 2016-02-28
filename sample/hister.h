#ifndef HISTER_H
#define HISTER_H

#include <vector>
#include <string>
#include <map>
#include <cstdlib>

template<typename T>
class Hister
{
public:
    Hister(){}
    virtual ~Hister(){}
    
    
    void reset(){ hist.clear(); }
    void addSample(T sample);
    void addSampleFile(std::string filename);
    void addHist(T data, unsigned freq);
    void addHistFile(std::string filename);

    
    std::vector<unsigned> getHist() const;             // return histogram
    void printHist() const;
    unsigned samplesN() const; // the number of words that have been read
    size_t distinctN() const { return hist.size(); }
    
private:
    std::map<T, unsigned> hist;
};


#include "hister.tpp"



#endif
