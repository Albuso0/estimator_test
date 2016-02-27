#include "samplegen.h"
#include <random>
#include <vector>

void SampleGen::discrete(int n, const std::vector<double> &p)
{
    std::discrete_distribution<int> distribution ( p.begin(),p.end() );
    for (int i=0; i<n; ++i)
        addSample( distribution(generator) );
}

