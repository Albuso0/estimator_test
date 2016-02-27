#ifndef SAMPLEGEN_H
#define SAMPLEGEN_H
#include <random>
#include <vector>
#include "hister.h"

class SampleGen: // Random sample generator and counter for DISCRETE distributions
    public Hister<int>
{
public:
    SampleGen(){}
    virtual ~SampleGen(){}
	
    void setSeed(int seed){ generator.seed( seed ); }
	
    void discrete(int n, const std::vector<double>& p);

private:
    std::default_random_engine generator;
	
};


#endif 
