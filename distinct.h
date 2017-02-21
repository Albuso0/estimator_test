#ifndef DISTINCT_H
#define DISTINCT_H

#include "finop.h"

class Distinct
{
public:
    Distinct(){}
    Distinct(int k);
    virtual ~Distinct(){}
	
    // Direct design, no sample splitting in theory
    double estimate() const;
    double getCoeff( int N ) const;
	
    // Other estimators
    double estimate_plug() const;  // Plug-in estimator

	
    void setk( int val ) { }     // CAREFUL: impact others
    void setc0( double val ) { } // CAREFUL: impact others 
    void setc1( double val ) { } // CAREFUL: impact others
    void setL( int val ) { L = val; }
    void setM( int val ) { M = val; }
    
    // set fingerprint, also update sample size
    void setFin( const std::string filename );
    void setFin( const vint &freq, const vint &cnt );
    // set fingerprint through histogram, also update sample size
    void setHist( const vint &hist );
    void setHist( const std::string filename );
    

    int getk() const { return k; }
    int getL() const { return L; }
    int getM() const { return M; }
    int getn() const { return n; }
    vfin getFin() const { return fin; } // get a copy of fingerprint
private:
    int k, L, M, n;  // matrix M-L. n samples from k-ball urn
    double c0, c1;
    vfin fin;        // Fingerprint(profile). Sorted fingerprint preferred
    std::vector<double> coeffs;
    
    // update n,L,M
    void update();
    void lse();
};


#endif

