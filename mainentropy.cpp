#include "entropy.h"
#include <iostream>
#include <algorithm>
#include "samplegen.h"
#include <vector>

#include "commandline.h"

void uniform();
void mix();
void zipf();

int main(int argc, char *argv[])
{
    // std::CommandLine cmd;
    // cmd.AddValue ("seed",  "", seed);
    // cmd.Parse (argc, argv);
    
    // uniform();
    mix();
    // zipf();
    
    return 0;
}

void zipf()
{
    double c0 = 1.3, c1 = 8, c2 = 1.5;
    int seed = 0;
    
    SampleGen gen;
    gen.reset();
    gen.setSeed( seed );
    
    int k = 1000000;
    std::vector<double> p( k );
    double norm = 0;
    for (int i = 0; i < k; ++i)
    {
        p[i] = 1.0/(i+1);
        norm += p[i];
    }
    double truth = 0;
    for (int i = 1; i <= k; ++i)
    {
        truth += 1.0/i/norm * log( i*norm );
    }
    truth /= log(2);
    
    Entropy entropy( k );
    entropy.setDegree( c0*log(k) );
    entropy.setInterval( c1*log(k) );
    entropy.setThreshold( c2*log(k) );
    printf("Zipf(1) distribution.\nAlphabet size=%d.\nPolynoimal degree=%d.\nApproximation interval=[0,%.2f/n].\nPlug-in threshold=%d.\n",
           k,entropy.getDegree(),entropy.getInterval(), (int)floor(entropy.getThreshold())+1);
    printf("\nSample size\tTruth(bits)\tPlug-in(bits)\tPolynomial(bits)\n");
    for ( int i = 0; i < 20; ++i)
    {
        gen.discrete( 10000, &p );
        entropy.setFin( gen.getFin() );
        printf("%d\t\t%.6f\t%.6f\t%.6f\n", entropy.getSampleSize(), truth, entropy.estimate_plug(), entropy.estimate());
    }
    return;
}

void mix()
{
    double c0 = 1.3, c1 = 8, c2 = 1.5;
    int seed = 0;
    
    SampleGen gen;
    gen.reset();
    gen.setSeed( seed );
    

    int k = 1000000;
    std::vector<double> p( k );

    for (int i = 0; i < k; i++)
        p[i] = 1;
    p[0] += k;
    double truth = log(2*k)-(k+1)*log(k+1)/2/k;
    truth /= log(2);
    
    Entropy entropy( k );
    entropy.setDegree( c0*log(k) );
    entropy.setInterval( c1*log(k) );
    entropy.setThreshold( c2*log(k) );
    printf("Mixture by half uniform and half point mass.\nAlphabet size=%d.\nPolynoimal degree=%d.\nApproximation interval=[0,%.2f/n].\nPlug-in threshold=%d.\n",
           k,entropy.getDegree(),entropy.getInterval(), (int)floor(entropy.getThreshold())+1);
    printf("\nSample size\tTruth(bits)\tPlug-in(bits)\tPolynomial(bits)\n");
    for ( int i = 0; i < 80; i++)
    {
        gen.discrete( 10000, &p );
        entropy.setFin( gen.getFin() );
        printf("%d\t\t%.6f\t%.6f\t%.6f\n", entropy.getSampleSize(), truth, entropy.estimate_plug(), entropy.estimate());
    }
    return;
}

void uniform()
{
    double c0 = 1.3, c1 = 8, c2 = 1.5;
    int seed = 0;
    
    int k = 1000000;
    int supp = k/2;
    
    SampleGen gen;
    gen.reset();
    gen.setSeed( seed );
    
    double truth = log(supp);
    truth /= log(2);

    Entropy entropy( k );
    entropy.setDegree( c0*log(k) );
    entropy.setInterval( c1*log(k) );
    entropy.setThreshold( c2*log(k) );
    printf("Uniform distributed on half of alphabet.\nAlphabet size=%d.\nPolynoimal degree=%d.\nApproximation interval=[0,%.2f/n].\nPlug-in threshold=%d.\n",
           k,entropy.getDegree(),entropy.getInterval(), (int)floor(entropy.getThreshold())+1);
    printf("\nSample size\tTruth(bits)\tPlug-in(bits)\tPolynomial(bits)\n");
    for ( int i = 0; i < 12; i++)
    {
        gen.uniform( 10000, supp );
        entropy.setFin( gen.getFin() );
        printf("%d\t\t%.6f\t%.6f\t%.6f\n", entropy.getSampleSize(), truth, entropy.estimate_plug(), entropy.estimate());
    }
    return;
}
