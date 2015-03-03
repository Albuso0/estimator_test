#ifndef SUPPORT_H
#define SUPPORT_H

#include <map>
#include <memory>
#include <boost/shared_array.hpp>

template <typename T>
class Support
{
public:
    Support();
	Support(int alpha);
    virtual ~Support(){}

	// Without the sample splitting required by theory
	double estimate(); // Hist must be given
	double estimate_poly();
	double estimate_plug();
	double getCoeff( double N ); // compute g(N). Interval must be given through setting rEnd or updating Hist.
	
	// Direct design, no sample splitting in theory
	double estimate2();
	double estimate_poly2();
	double getCoeff2( double N );
	
	// Other estimators
	double estimate_TG();
	double estimate_JK( int order );
	double estimate_CL1();
	double estimate_CL2();

	
	void setN( int _n ); // set n, update rEnd, a
	void setCN( double N_threshold ){ c_N = N_threshold; }
	void setCP( double p_threshold ); // set c_p, if n > 0 update rEnd, a
	void setCL( double L_threshold ); // set c_L, update L, new a, if n > 0 update a
	void setHist(std::shared_ptr< const std::map<T, int> > hist); // update n, rEnd = c_p * log(k) / n, a
	void setFin(std::shared_ptr< const std::map<int, int> > fin);

	int getN() { return n; }
	int getL() { return L; }
private:
	double c_p, c_N, c_L;
	int k, L, n;
	std::shared_ptr< const std::map<int, int> > mpFin;
	
	
	double lEnd, rEnd; // end points of interval. lEnd = 1/k, rEnd = c_p * log(k) / n 
	boost::shared_array<double> a; // 1- cos L arccos( x - (rEnd+lEnd)/(rEnd-lEnd) ) / cos L arccos( - (rEnd+lEnd)/(rEnd-lEnd) ) = sum_i a[i]*x^i
	void updateArr(); // update a
	// g(N) = sum_i a[i] * ( 2 / n / (rEnd-lEnd) )^i * (N)_i
	// N~Poi(np), E[g(N)] = sum_i a[i] * ( 2p/(rEnd-lEnd) )^i = 1- cos L arccos( 2p/(rEnd-lEnd) - (rEnd+lEnd)/(rEnd-lEnd) ) / cos L arccos( - (rEnd+lEnd)/(rEnd-lEnd) )
};


#endif

