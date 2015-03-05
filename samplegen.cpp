#include "samplegen.h"



void SampleGen::addSample(int data)
{
	std::map<int, int>::iterator it = mpHist->find( data );
	if ( it == mpHist->end() )
		mpHist->insert( std::pair<int,int>( data ,1 ) );
	else
		++(it->second);
}

void SampleGen::uniform(int n, int k)
{
	std::uniform_int_distribution<int> distribution( 0, k-1 );
	sampling( distribution, n );
}

void SampleGen::Poisson_truncated(int n, double lamdba, int min, int max)
{
	std::poisson_distribution<int> distribution(lamdba); 
	sampling_truncate( distribution, n, min, max);
}

void SampleGen::discrete(int n, std::vector<double>* p)
{
	std::discrete_distribution<int> distribution ( p->begin(),p->end() );
	sampling( distribution, n );
}

void SampleGen::negative_binomial(int n, int k, double p)
{
	std::negative_binomial_distribution<int> distribution( k, p );
	sampling( distribution, n );
}

SampleGen::SampleGen()
	:mpHist(new std::map<int, int>)
{
}