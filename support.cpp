#include "support.h"
#include "chebmore.h"
#include <cmath>
#include <string>
#include <iostream>
#include <algorithm>
#include <boost/math/special_functions/binomial.hpp>

/* ----------------------POLYNOMIAL ESTIMATOR-------------------------------- */
template <typename T>
double Support<T>::estimate()
{
    if ( n < c_p * k * log(k) ) // const
        return estimate_poly();
    else
        return estimate_plug();
}

template <typename T>
double Support<T>::estimate_poly()
{
    double result = 0.0;
    for ( std::map<int,int>::const_iterator it = mpFin->begin(); it != mpFin->end(); ++it )
        if ( it->first < c_N * log(k) ) 
        {
            result += getCoeff(it->first) * it->second;
        }
        else
        {
            result += it->second;
        }
    return result;
}

template <typename T>
double Support<T>::getCoeff( double N )
{
    double s = 2 / ( c_p * log(k) - n / k );
    // double s = 2 / n / (rEnd-lEnd);
    double gL = a[L];
    for (int i = L - 1; i>=0; i--)
        gL = a[i] + gL * (N-i) * s;
    return gL;
}
/* ----------------------END POLYNOMIAL ESTIMATOR-------------------------------- */


/* ----------------------POLYNOMIAL ESTIMATOR NO SPLITTING-------------------------------- */
template <typename T>
double Support<T>::estimate2()
{
    if ( n < c_p * k * log(k) ) // const
        return estimate_poly2();
    else
        return estimate_plug();
}

template <typename T>
double Support<T>::estimate_poly2()
{
    double result = 0.0;
    for ( std::map<int,int>::const_iterator it = mpFin->begin(); it != mpFin->end(); ++it )
        if ( it->first <= L ) 
        {
            result += getCoeff2(it->first) * it->second;
        }
        else
        {
            result += it->second;
        }
    return result;
}
template <typename T>
double Support<T>::getCoeff2( double N )
{
    if (N==0) return 0;
	
    double s = 2 / ( c_p * log(k) - n / k );
    // double s = 2 / n / (rEnd-lEnd);
    double gL = 1;
    for ( int i = 1; i <= N; ++i )
        gL *= i * s;
    return (1+gL*a[(int)N]);
}
/* ----------------------END POLYNOMIAL ESTIMATOR NO SPLITTING-------------------------------- */





/* ----------------------OTHER ESTIMATORS-------------------------------- */
template <typename T>
double Support<T>::estimate_plug()
{
    double result = 0.0;
    for ( std::map<int,int>::const_iterator it = mpFin->begin(); it != mpFin->end(); ++it )
        result += it->second;
    return result;
}

template <typename T>
double Support<T>::estimate_TG()
{
    std::map<int, int>::const_iterator it = mpFin->find( 1 );
    double n1 = ( it == mpFin->end() ) ? ( 0 ) : ( it->second );
	
    return estimate_plug() / ( 1 - n1/n );
}

template <typename T>
double Support<T>::estimate_JK( int order )
{
    double result = estimate_plug();
    for ( std::map<int,int>::const_iterator it = mpFin->begin(); it != mpFin->end(); ++it )
    {
        int j = it->first;
        if ( j > order )
            break;
        int nj = it->second;
        int adjust = boost::math::binomial_coefficient<double>( order , j ) * nj;
        result = ( j % 2 == 0 ) ? ( result - adjust ) : ( result + adjust ) ; 
    }
    return result;
}

template <typename T>
double Support<T>::estimate_CL1()
{
    std::map<int, int>::const_iterator it = mpFin->find( 1 );
    double n1 = ( it == mpFin->end() ) ? ( 0 ) : ( it->second );
	
    double N1 = estimate_plug() / ( 1 - n1/n );

    double sum = 0.0;
    for ( std::map<int,int>::const_iterator it = mpFin->begin(); it != mpFin->end(); ++it )
    {
        int j = it->first;
        int fj = it->second;
        sum += j * ( j-1 ) * fj;
        // std::cout<< sum <<" "<<j << " "<<fj<<std::endl;
    }
    double h_gamma_sq = std::max( N1 * sum / n / (n-1) , 0.0 );
	
    return N1 + n * n1 / ( n - n1 ) * h_gamma_sq;
}

template <typename T>
double Support<T>::estimate_CL2()
{
    std::map<int, int>::const_iterator it = mpFin->find( 1 );
    double n1 = ( it == mpFin->end() ) ? ( 0 ) : ( it->second );
	
    double N1 = estimate_plug() / ( 1 - n1/n );

    double sum = 0.0;
    for ( std::map<int,int>::const_iterator it = mpFin->begin(); it != mpFin->end(); ++it )
    {
        int j = it->first;
        int fj = it->second;
        sum += j * ( j-1 ) * fj;
    }
	
    double h_gamma_sq1 = std::max( N1 * sum / n / (n-1) , 0.0 );

    double h_gamma_sq = std::max( h_gamma_sq1 * ( 1 + n1 * sum / (n-1) / (n-n1) ), 0.0 );
	
    return N1 + n * n1 / ( n - n1 ) * h_gamma_sq;
}

template <typename T>
double Support<T>::estimate_sinc()
{
    double sum = 0.0;
    for ( std::map<int,int>::const_iterator it = mpFin->begin(); it != mpFin->end(); ++it )
    {
        int j = it->first;
        int fj = it->second;
        if ( it->first <= L )
        {
            double gj = 0;
            double multi = -1.0;
            for ( int i = 1; 2*i <= j; i++ )
            {
                multi *= - (3.1415926*k/n) * (3.1415926*k/n);
                gj += boost::math::binomial_coefficient<double>( j , 2*i ) * multi / (2*i+1);
            }
            sum += gj * fj;
        }
        else
        {
            sum += fj;
        }
    }
	
    return sum;
}
/* ----------------------END OTHER ESTIMATORS-------------------------------- */





template <typename T>
Support<T>::Support()
{
    c_p = 1.0;
    c_N = 0.5;
    c_L = 1.0;
}

template <typename T>
Support<T>::Support(int alpha)
    :Support()
{
    k = alpha;
    n = 0;
    L = c_L * log(k);
    a = boost::shared_array<double>(new double[L+1]);
    lEnd = 1.0 / k;
}

template <typename T>
void Support<T>::setHist(std::shared_ptr< const std::map<T, int> > hist) 
{
    std::shared_ptr< std::map<int, int> > pHistHist( new std::map<int, int> );
    n = 0;
    for ( typename std::map<T,int>::const_iterator it = hist->begin(); it != hist->end(); ++it )
    {
        int freq = it->second;
        n += freq;
        std::map<int,int>::iterator iter = pHistHist->find( freq );
        if ( iter == pHistHist->end() )
            pHistHist->insert( std::pair<int,int>( freq,1 ) );
        else
            ++(iter->second);
    }
    mpFin = pHistHist;
    if ( n > 0 )
    {
        rEnd = std::min<double>(1, c_p * log(k) / n);
        updateArr();
    }
}
template <typename T>
void Support<T>::setFin(std::shared_ptr< const std::map<int, int> > fin) 
{
    mpFin = fin;
    n = 0;
    for ( std::map<int,int>::const_iterator it = mpFin->begin(); it != mpFin->end(); ++it )
        n += it->first * it->second;
    if ( n > 0 )
    {
        rEnd = std::min<double>(1, c_p * log(k) / n);
        updateArr();
    }
}

template <typename T>
void Support<T>::updateArr() // the array a has length L = c_L * log(k), and depends on lEnd = 1 / k, rEnd = c_p * log(k) / n and L
{
    if ( n == 0 ) std::cerr<<"No sample!"<<std::endl;
    if ( rEnd <= lEnd )
    {
        for (unsigned i = 0;i < L+1;i++) a[i] = 0;
        return;
    }
    double A = ( rEnd + lEnd ) / ( rEnd - lEnd );
	
    ChebMore cheb(L, 1, -A); // polynomial of cos L arccos(t-A)
    boost::shared_array<const double> a0 = cheb.expand(); // Expand: cos L arccos(x-A)=sum_i a0[i]*x^i
    double amp = cheb.evaluate(0); // cos L arccos(-A)

    // Expand: 1- cos L arccos(t-A)/cos L arccos(-A) = sum_i a[i]*x^i
    for (unsigned i = 0;i < L+1;i++) a[i] = - a0[i] / amp;
    a[0] += 1;
    a[0] = 0;

    // for ( int i = 0; i <= L; i++ ) std::cout<<a[i]<<std::endl;
}



template <typename T>
void Support<T>::setN( int _n )
{
    n = _n;
    if ( n > 0 )
    {
        rEnd = std::min<double>(1, c_p * log(k) / n);
        updateArr();
    }
}
template <typename T>
void Support<T>::setCP( double p_threshold )
{
    c_p = p_threshold;
    if ( n > 0 )
    {
        rEnd = std::min<double>(1, c_p * log(k) / n);
        updateArr();
    }
}
template <typename T>
void Support<T>::setCL( double L_threshold )
{
    c_L = L_threshold;
    L = c_L * log(k);
    a = boost::shared_array<double>(new double[L+1]);
    if ( n > 0 )
        updateArr();
}

template class Support<std::string>;
template class Support<int>;
