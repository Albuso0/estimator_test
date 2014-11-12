#include "chebmore.h"
#include <cmath>

unsigned long ChebMore::binom(unsigned long n, unsigned long k)
{
    unsigned long i;
    unsigned long b;
    if (0 == k || n == k) {
        return 1;
    }
    if (k > n) {
        return 0;
    }
    if (k > (n - k)) {
        k = n - k;
    }
    if (1 == k) {
        return n;
    }
    b = 1;
    for (i = 1; i <= k; ++i) {
        b *= (n - (k - i));
        if (b < 0) return -1; /* Overflow */
        b /= i;
    }
    return b;
}


boost::shared_array< const double > ChebMore::expand_basic() // Expansion of cos(L arccos x)
{
	for (int i = 0; i < L + 1; i++)
		a[i] = 0;
	
	if (L % 2 == 0)
	{
		unsigned n = L / 2;
		for (unsigned k = 0; k <= n; k++)
			a[2*k] = pow(-1,n-k) * pow(2,2*k) * n / (n+k) * binom(n+k,2*k); // may be simpler
	}
	else
	{
		unsigned n = (L-1)/2;
		for (unsigned k = 0; k <= n; k++)
			a[2*k+1] = pow(-1,n-k) * pow(2,2*k) * (2*n+1) / (2*k+1) * binom(n+k,2*k); // may be simpler
	}
	return a;
}
boost::shared_array< const double > ChebMore::expand() // Expansion of cos(L arccos (bx+c))
{
	expand_basic();
	boost::shared_array<double> a0(new double[L+1]);
	for (int i = 0; i < L + 1; i++)
	{
		a0[i] = a[i];
		a[i] = 0;
	}
	
	// sum_i a[i] x^i = sum_i a0[i]*(bx+c)^i
	for (int i = 0; i <= L; i++)
		for (int j = 0; j <= i; j++)
			a[j] += a0[i] * pow(b,j) * pow(c,i-j) * binom(i,j);
	return a;
}

double ChebMore::evaluate(double x)
{
	double A = b * x + c;
	if ( A > 1 )
	{
		return cosh(L*acosh(A));
	}
	else if ( A >= -1 )
	{
		return cos(L*acos(A));
	}
	else
	{
		return ( cosh(L*acosh(-A)) * ( (L%2==0)? 1:-1)) ;
	}
}
