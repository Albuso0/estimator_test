#ifndef SUPPORT_H
#define SUPPORT_H

#include <map>
#include <memory>
#include <boost/shared_array.hpp>

template <typename T>
class Support
{
public:
    Support(){}
	Support(int alpha);
    virtual ~Support(){}
	
	double estimate(); // Hist must be given
	double getCoeff( int N ); // compute g(N). Interval must be given through setting rEnd or updating Hist.
	
	void setN( int _n ); // set n, update rEnd, a
	void setHist(std::shared_ptr< const std::map<T, int> > hist); // update n, rEnd = c_th * log(k) / n, a
private:
	int k, L, n;
	double c_th;
	std::shared_ptr< const std::map<T, int> > mpHist;
	
	double estimate_poly();
	double estimate_plug(){ return mpHist->size(); }

	double lEnd, rEnd; // end points of interval. lEnd = 1/k, rEnd = c_th * log(k) / n 
	boost::shared_array<double> a; // 1- cos L arccos( x - (rEnd+lEnd)/(rEnd-lEnd) ) / cos L arccos( - (rEnd+lEnd)/(rEnd-lEnd) ) = sum_i a[i]*x^i
	void updateArr(); // update a
	// g(N) = sum_i a[i] * ( 2 / n / (rEnd-lEnd) )^i * (N)_i
	// N~Poi(np), E[g(N)] = sum_i a[i] * ( 2p/(rEnd-lEnd) )^i = 1- cos L arccos( 2p/(rEnd-lEnd) - (rEnd+lEnd)/(rEnd-lEnd) ) / cos L arccos( - (rEnd+lEnd)/(rEnd-lEnd) )
};


#endif

