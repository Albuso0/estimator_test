// samplegen.h
#ifndef SAMPLEGEN_H
#define SAMPLEGEN_H
#include <random>
#include <vector>
#include "hister.h"

class SampleGen // Random sample generator and counter for DISCRETE distributions
{
public:
    SampleGen(){}
    virtual ~SampleGen(){}

    
    void setSeed(int seed){ d_generator.seed( seed ); }        // set seed to random number generator
    void discrete(unsigned n, const std::vector<double>& p);   // generate n samples from (unnormalized) distribution vector p  
    void resetHist(){ d_hist.reset(); }

    
    std::vector<unsigned> getHist() const { return d_hist.getHist(); }
    void printHist() const { d_hist.printHist(); }
    unsigned samplesN() const { return d_hist.samplesN(); } // the number of words that have been read
    size_t distinctN() const { return d_hist.distinctN(); }

private:
    Hister<unsigned> d_hist;
    std::default_random_engine d_generator;
	
};


#endif 
