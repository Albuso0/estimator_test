#include "support.h"
#include <cmath>

template <typename T>
double Support<T>::estimate()
{
	if ( n < k * log(k) ) // const
		return estimate_poly();
	else
		return estimate_plug();
}

template <typename T>
double Support<T>::estimate_poly()
{
	return 0;
}

template <typename T>
double Support<T>::estimate_plug()
{
	return 0;
}
