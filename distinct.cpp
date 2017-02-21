#include "distinct.h"
#include "finop.h"
#include <cmath>
#include <string>
#include <algorithm>




/* ----------------------POLYNOMIAL ESTIMATOR-------------------------------- */
double Distinct::estimate() const
{
    if ( M > 0 ) 
    {
        double result = 0.0;
        for ( const auto & pair : fin )
                result += getCoeff(pair.first) * pair.second;
        return result;
    }
    else
        return estimate_plug();
}

double Distinct::getCoeff( int N ) const
{
    if ( N == 0 )
        return 0;
    else if ( N > L )
        return 1;
    else
        return coeffs[N-1];
}
/* ----------------------END POLYNOMIAL ESTIMATOR-------------------------------- */





/* ----------------------OTHER ESTIMATORS-------------------------------- */
double Distinct::estimate_plug() const
{
    double result = 0.0;
    for ( const auto & pair : fin )
        result += pair.second;
    return result;
}
/* ----------------------END OTHER ESTIMATORS-------------------------------- */



Distinct::Distinct(int size)
    :Distinct()
{
    k = size;
    c0 = 0.45;
    c1 = 0.5;
    L = c0 * log(k);
}


void Distinct::update()
{
    n = getSampleSize(fin);
    M = c1 * k * log(k) / n;
    L = c0 * log(k);
    if (L>M)
        L = M;
    coeffs.resize(L);
}


void Distinct::setFin( const std::string filename )
{
    ::setFin(fin, filename);
    update();
}
void Distinct::setFin( const vint &freq, const vint &cnt )
{
    ::setFin(fin, freq, cnt);
    update();
}
void Distinct::setHist( const vint &hist )
{
    ::setHist(fin, hist);
    update();
}
void Distinct::setHist( const std::string filename )
{
    ::setHist(fin, filename);
    update();
}
