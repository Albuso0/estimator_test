#ifndef CHEBMORE_H
#define CHEBMORE_H
#include <memory>
#include <vector>
#include <boost/math/special_functions/binomial.hpp>
#include <boost/shared_array.hpp>


class ChebMore  // Polynomial in the form cos(L arccos (bx+c))
{
public:
    ChebMore(){}
	ChebMore(int _L, double _b = 1, double _c = 0):L(_L),b(_b),c(_c),a(new double[L+1]){}
    virtual ~ChebMore(){}
	boost::shared_array< const double > expand_basic(); // Expansion of cos(L arccos x)
	boost::shared_array< const double > expand(); // Expansion of cos(L arccos (bx+c))
	double evaluate(double x);

private:
	unsigned long binom(unsigned long n, unsigned long k);
	int L;
	double b,c;
	boost::shared_array<double> a;
};

#endif
