#include "support.h"
#include "chebmore.h"
#include <cmath>
#include <string>
#include <iostream>
#include <algorithm>


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
	std::map<int, int> HistHist;
	for ( typename std::map<T,int>::const_iterator it = mpHist->begin(); it != mpHist->end(); ++it )
	{
		int freq = it->second;
		std::map<int,int>::iterator iter = HistHist.find( freq );
		if ( iter == HistHist.end() )
			HistHist.insert( std::pair<int,int>( freq,1 ) );
		else
			++(iter->second);
	}
	double result = 0.0;
	for ( std::map<int,int>::iterator it = HistHist.begin(); it != HistHist.end(); ++it )
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
	mpHist = hist;
	n = 0;
	for ( typename std::map<T,int>::const_iterator it = mpHist->begin(); it != mpHist->end(); ++it )
		n += it->second;
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
double Support<T>::getCoeff( int N )
{
	double s = 2 / ( c_p * log(k) - n / k );
	// double s = 2 / n / (rEnd-lEnd);
	double gL = a[L];
	for (int i = L - 1; i>=0; i--)
		gL = a[i] + gL * (N-i) * s;
	return gL;
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
