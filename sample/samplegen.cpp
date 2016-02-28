// samplegen.cpp
#include "samplegen.h"
#include <random>
#include <vector>

void SampleGen::discrete(unsigned n, const std::vector<double> &p)
{
    std::discrete_distribution<unsigned> distribution ( p.begin(),p.end() );
    for (unsigned i=0; i<n; ++i)
        d_hist.addSample( distribution(d_generator) );
}

