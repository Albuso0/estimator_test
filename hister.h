#ifndef HISTER_H
#define HISTER_H

#include <cstdlib>
#include <vector>
#include <string>
#include <map>

template<typename T>
class Hister
{
public:
    Hister(){}
    virtual ~Hister(){}

    std::vector<int> getHist() const;             // return histogram
    void printHist() const;

    std::size_t samplesN() const; // the number of words that have been read
    std::size_t distinctN() const { return hist.size(); }

protected:
    void resetHist(){ hist.clear(); }
	
    std::map<std::string, int> hist;
    
    void addSample(T sample);
    void addSampleFile(std::string filename);
    
    void addHist(T data, int freq);
    void addHistFile(std::string filename);
};


#include "hister.tpp"



#endif
