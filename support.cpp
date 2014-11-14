#include "support.h"
#include "chebmore.h"
#include <cmath>
#include <string>
#include <iostream>


template <typename T>
double Support<T>::estimate()
{
	if ( n < c_th * k * log(k) ) // const
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
		if ( it->first < 0.5 * log(k) ) // const!!!
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
Support<T>::Support(int alpha)
	:k(alpha),
	 L(log(k)), // const
	 a(new double[L+1])
{
	c_th = 1.0;
	lEnd = 1.0 / k;
}

template <typename T>
void Support<T>::setHist(std::shared_ptr< const std::map<T, int> > hist) 
{
	mpHist = hist;
	n = 0;
	for ( typename std::map<T,int>::const_iterator it = mpHist->begin(); it != mpHist->end(); ++it )
		n += it->second;
	rEnd = c_th * log(k) / n;
	updateArr();
}

template <typename T>
void Support<T>::updateArr()
{
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
	double s = 2 / ( c_th * log(k) - n / k );
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
	rEnd = c_th * log(k) / n;
	updateArr();
}

template class Support<std::string>;
