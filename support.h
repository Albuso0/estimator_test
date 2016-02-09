#ifndef SUPPORT_H
#define SUPPORT_H

#include <map>
#include <memory>
#include <boost/shared_array.hpp>

class Support
{
public:
    Support();
    Support(double pmin);
    virtual ~Support(){}

	
    // Direct design, no sample splitting in theory
    double estimate() const;
    double getCoeff( int N ) const;
	
    // Other estimators
    double estimate_plug() const;
    double estimate_TG() const;
    double estimate_JK( int order ) const;
    double estimate_CL1() const;
    double estimate_CL2() const;
    double estimate_sinc() const;
    double estimate_old() const;
    double getCoeff_old( double N ) const;

	
    void setPmin( double p_min ) { pmin = p_min; }
    void setInterval( double rEnd ){ Ratio = rEnd; }
    void setDegree( int deg ) { L = deg; }
    void setThreshold( double N_threshold ){ N_thr = N_threshold; }
    
    void setFin( std::shared_ptr< const std::map<int, int> > ptr_fin_map );
    void setHist( const std::vector<int> &hist );

    int getN() const { return n; }
    int getL() const { return L; }
private:
    double pmin;  // =1/k. Minimum non-zero mass
    int L;        // =c0*log(k). Degree of polynomial
    double Ratio; // =c1*log(k). Approximation over [pmin,c1*log(k)/n]
    int n; // sample size
    
    int N_thr; // =c2*log(k). Used in old estimate_old. Threshold to apply polynomial estimator 
	
    std::vector< std::pair<int, int> > fin; // Fingerprint(profile). sorted
    
    // std::vector<double> a; // 1- cos L arccos( x - (rEnd+lEnd)/(rEnd-lEnd) ) / cos L arccos( - (rEnd+lEnd)/(rEnd-lEnd) ) = sum_i a[i]*x^i
    // void updateArr(); // update a
    // g(N) = sum_i a[i] * ( 2 / n / (rEnd-lEnd) )^i * (N)_i
    // N~Poi(np), E[g(N)] = sum_i a[i] * ( 2p/(rEnd-lEnd) )^i = 1- cos L arccos( 2p/(rEnd-lEnd) - (rEnd+lEnd)/(rEnd-lEnd) ) / cos L arccos( - (rEnd+lEnd)/(rEnd-lEnd) )
};


#endif

