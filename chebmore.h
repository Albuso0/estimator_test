#ifndef CHEBMORE_H
#define CHEBMORE_H
#include <memory>
#include <vector>


class ChebMore  // Polynomial in the form cos(L arccos (bx+c))=sum a_i * x^i
{
public:
    ChebMore(){}
    ChebMore(int _L, double _b = 1, double _c = 0):L(_L),b(_b),c(_c){}
    virtual ~ChebMore(){}
    

    std::vector<double> expand_basic() const; // Expansion of cos(L arccos x)
    std::vector<double> expand() const;       // Expansion of cos(L arccos (bx+c))
    
    double evaluate(double x) const; // Evaluation of cos(L arccos (bx+c))

private:
    int L;
    double b,c;
    long long binom(unsigned n, unsigned k) const;
};

#endif
