#include "support.h"
#include "chebmore.h"
#include <cmath>
#include <string>
#include <iostream>
#include <algorithm>
#include <boost/math/special_functions/binomial.hpp>

/* ----------------------POLYNOMIAL ESTIMATOR-------------------------------- */

double Support::estimate_old() const
{
    if ( pmin < Ratio / n ) // const
    {
        double result = 0.0;
        for ( const auto & pair : fin )
            if ( pair.first < N_thr ) 
            {
                result += getCoeff_old(pair.first) * pair.second;
            }
            else
            {
                result += pair.second;
            }
        return result;
    }
    else
        return estimate_plug();
}

double Support::getCoeff_old( double N ) const
{
    double rEnd = Ratio / n;
    double lEnd = pmin;
    double A = ( rEnd + lEnd ) / ( rEnd - lEnd );
    double a[L+1];
	
    ChebMore cheb(L, 1, -A); // polynomial of cos L arccos(t-A)
    boost::shared_array<const double> a0 = cheb.expand(); // Expand: cos L arccos(x-A)=sum_i a0[i]*x^i
    double amp = cheb.evaluate(0); // cos L arccos(-A)

    // Expand: 1- cos L arccos(t-A)/cos L arccos(-A) = sum_i a[i]*x^i
    for (unsigned i = 0;i < L+1;i++) a[i] = - a0[i] / amp;
    a[0] += 1;
    a[0] = 0;
    
    double s = 2 / ( Ratio - n * pmin );
    // double s = 2 / n / (rEnd-lEnd);
    double gL = a[L];
    for (int i = L - 1; i>=0; i--)
        gL = a[i] + gL * (N-i) * s;
    return gL;
}


/* ----------------------END POLYNOMIAL ESTIMATOR-------------------------------- */




/* ----------------------POLYNOMIAL ESTIMATOR NO SPLITTING-------------------------------- */
double Support::estimate() const
{
    if ( pmin < Ratio / n ) // const
    {
        double result = 0.0;
        for ( const auto & pair : fin )
            if ( pair.first <= L ) 
            {
                result += getCoeff(pair.first) * pair.second;
            }
            else
            {
                result += pair.second;
            }
        return result;
    }
    else
        return estimate_plug();
}

double Support::getCoeff( int N ) const
{
    if (N==0) return 0;
	
    double s = 2 / ( Ratio - n * pmin );
    // double s = 2 / n / (rEnd-lEnd);
    double gL = 1;
    for ( int i = 1; i <= N; ++i )
        gL *= i * s;

    double rEnd = Ratio / n;
    double lEnd = pmin;
    double A = ( rEnd + lEnd ) / ( rEnd - lEnd );
    double a[L+1];
	
    ChebMore cheb(L, 1, -A); // polynomial of cos L arccos(t-A)
    boost::shared_array<const double> a0 = cheb.expand(); // Expand: cos L arccos(x-A)=sum_i a0[i]*x^i
    double amp = cheb.evaluate(0); // cos L arccos(-A)

    // Expand: 1- cos L arccos(t-A)/cos L arccos(-A) = sum_i a[i]*x^i
    for (unsigned i = 0;i < L+1;i++) a[i] = - a0[i] / amp;
    a[0] += 1;
    a[0] = 0;


    return (1+gL*a[N]);
}
/* ----------------------END POLYNOMIAL ESTIMATOR NO SPLITTING-------------------------------- */





/* ----------------------OTHER ESTIMATORS-------------------------------- */

double Support::estimate_plug() const
{
    double result = 0.0;
    for ( const auto & pair : fin )
        result += pair.second;
    return result;
}


double Support::estimate_TG() const
{
    double n1 = 0;
    for ( const auto & pair : fin )
        if ( pair.first == 1 )
        {
            n1 = pair.second;
            break;
        }
	
    return estimate_plug() / ( 1 - n1/n );
}


double Support::estimate_JK( int order ) const
{
    double result = estimate_plug();
    for ( const auto & pair : fin )
    {
        int j = pair.first;
        if ( j > order )
            break;
        int nj = pair.second;
        int adjust = boost::math::binomial_coefficient<double>( order , j ) * nj;
        result = ( j % 2 == 0 ) ? ( result - adjust ) : ( result + adjust ) ; 
    }
    return result;
}


double Support::estimate_CL1() const
{
    double n1 = 0;
    for ( const auto & pair : fin )
        if ( pair.first == 1 )
        {
            n1 = pair.second;
            break;
        }
	
    double N1 = estimate_plug() / ( 1 - n1/n );

    double sum = 0.0;
    for ( const auto & pair : fin )
    {
        int j = pair.first;
        int fj = pair.second;
        sum += j * ( j-1 ) * fj;
        // std::cout<< sum <<" "<<j << " "<<fj<<std::endl;
    }
    double h_gamma_sq = std::max( N1 * sum / n / (n-1) , 0.0 );
	
    return N1 + n * n1 / ( n - n1 ) * h_gamma_sq;
}


double Support::estimate_CL2() const
{
    double n1 = 0;
    for ( const auto & pair : fin )
        if ( pair.first == 1 )
        {
            n1 = pair.second;
            break;
        }
	
    double N1 = estimate_plug() / ( 1 - n1/n );

    double sum = 0.0;
    for ( const auto & pair : fin )
    {
        int j = pair.first;
        int fj = pair.second;
        sum += j * ( j-1 ) * fj;
    }
	
    double h_gamma_sq1 = std::max( N1 * sum / n / (n-1) , 0.0 );

    double h_gamma_sq = std::max( h_gamma_sq1 * ( 1 + n1 * sum / (n-1) / (n-n1) ), 0.0 );
	
    return N1 + n * n1 / ( n - n1 ) * h_gamma_sq;
}


double Support::estimate_sinc() const
{
    double sum = 0.0;
    double k = 1.0/pmin;
    for ( const auto & pair : fin )
    {
        int j = pair.first;
        int fj = pair.second;
        if ( pair.first <= L )
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




Support::Support()
{
}


Support::Support(double p_min)
    :Support()
{
    pmin = p_min;
}

void Support::setHist( const std::vector<int> &hist )
{
    fin.clear();
    n = 0;
    
    std::map<int, int> fin_map;
    for ( const auto & freq : hist )
    {
        auto iter = fin_map.find( freq );
        if ( iter == fin_map.end() )
            fin_map.insert( std::make_pair( freq,1 ) );
        else
            ++(iter->second);
    }
    
    for ( auto it = fin_map.begin(); it != fin_map.end(); ++it )
    {
        int freq = it->first, cnt = it->second;
        fin.push_back( std::make_pair( freq, cnt ) );
        n += (freq * cnt);
    }
}


void Support::setFin(std::shared_ptr< const std::map<int, int> > ptr_fin_map) 
{
    fin.clear();
    n = 0;
    for ( auto it = ptr_fin_map->begin(); it != ptr_fin_map->end(); ++it )
    {
        int freq = it->first, cnt = it->second;
        fin.push_back( std::make_pair( freq, cnt ) );
        n += (freq * cnt);
    }
}

