#ifndef HISTER_H
#define HISTER_H

#include <vector>
#include <string>
#include <map>

template<typename T>
class Hister
{
public:
    Hister(){}
    virtual ~Hister(){}
    
    std::vector<unsigned> getHist() const;             // return histogram
    void printHist() const;

    unsigned samplesN() const; // the number of words that have been read
    size_t distinctN() const { return hist.size(); }
    
    void resetHist(){ hist.clear(); }
    void addSampleFile(std::string filename);
    void addHistFile(std::string filename);
protected:
	
    std::map<T, unsigned> hist;
    
    void addSample(T sample);
    void addHist(T data, unsigned freq);
};


#include "hister.tpp"



#endif
