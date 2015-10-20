#include "entropy.h"
#include <iostream>
#include <algorithm>
#include "samplegen.h"
#include <vector>

#include "commandline.h"

void uniform();
void mix();
void zipf();
void uniform_regime();
void zipf_regime();

typedef std::map<int, int> Fingerprint;
// enum TEST { uniform, mix, zipf1 };

int main(int argc, char *argv[])
{
    // std::CommandLine cmd;
    // cmd.AddValue ("seed",  "", seed);
    // cmd.Parse (argc, argv);

    // uniform_regime();
    // mix();
    // zipf();
    
    return 0;
}

// void TEST_regime(TEST distr)
// {
//     double c0 = 1.3, c1 = 8, c2 = 1.5;
//     int trials = 20;

//     int k = 100;
//     printf("Regime: sample size=50*support size.\n");
//     printf("Unit: bits.\n");
//     printf("\nSupport size\tSample size\tTruth\it\tRMSE:Plug\tRMSE:MM\tRMSE:Poly\n");

//     while (k < 1000)
//     {
//         Entropy entropy( k );
//         entropy.setDegree( c0*log(k) );
//         entropy.setInterval( c1*log(k) );
//         entropy.setThreshold( c2*log(k) );
//         SampleGen gen;

//         std::vector<double> p( k );
//         double truth = 0;
//         switch( distr )
//         {
//         case(uniform):
//             truth = log(k)/log(2);
//             for (int i = 0; i < k; ++i)
//                 p[i] = 1.0/k;
//             break;
//         case(zipf1):
//             double norm = 0.0;
//             for (int i = 0; i < k; ++i)
//             {
//                 p[i] = 1.0/(i+1);
//                 norm += p[i];
//             }
//             for (int i = 1; i <= k; ++i)
//                 truth += 1.0/double(i)/norm * log( double(i)*norm );
//             truth /= log(2);
//             break;
//         }
        
//         int n = 50*k;
//         double SE_plug = 0;
//         double SE_poly = 0;
//         for (int seed = 0; seed < trials; ++seed)
//         {
//             gen.reset();
//             gen.setSeed( seed );
//             gen.discrete( n, &p );
//             entropy.setFin( gen.getFin() );
//             SE_plug += pow(truth-entropy.estimate_plug(),2);
//             SE_poly += pow(truth-entropy.estimate(),2);
//         }
//         printf("%d\t\t%d\t\t%.6f\t%.6f\t%.6f\n", k, n, truth, sqrt(SE_plug/trials), sqrt(SE_poly/trials));
//         k = floor(double(k)*1.1);
//     }
    
//     return;
// }


void zipf_regime()
{
    double c0 = 1.3, c1 = 8, c2 = 1.5;
    int trials = 20;

    int k = 100;
    printf("\nSupport size\tSample size\tTruth(bits)\tPlug-in(bits)\tPolynomial(bits)\n");
    while (k < 1000)
    {
        Entropy entropy( k );
        entropy.setDegree( c0*log(k) );
        entropy.setInterval( c1*log(k) );
        entropy.setThreshold( c2*log(k) );
        SampleGen gen;

        std::vector<double> p( k );
        double norm = 0.0;
        for (int i = 0; i < k; ++i)
        {
            p[i] = 1.0/(i+1);
            norm += p[i];
        }
        double truth = 0;
        for (int i = 1; i <= k; ++i)
        {
            truth += 1.0/double(i)/norm * log( double(i)*norm );
        }
        truth /= log(2);
        
        int n = 50*k;
        double SE_plug = 0;
        double SE_poly = 0;
        for (int seed = 0; seed < trials; ++seed)
        {
            gen.reset();
            gen.setSeed( seed );
            gen.discrete( n, &p );
            entropy.setFin( gen.getFin() );
            SE_plug += pow(truth-entropy.estimate_plug(),2);
            SE_poly += pow(truth-entropy.estimate(),2);
        }
        printf("%d\t\t%d\t\t%.6f\t%.6f\t%.6f\n", k, n, truth, sqrt(SE_plug/trials), sqrt(SE_poly/trials));
        k = floor(double(k)*1.1);
    }
    
    return;
}

void uniform_regime()
{
    double c0 = 1.3, c1 = 8, c2 = 1.5;
    int trials = 20;

    int k = 100;
    printf("\nSupport size\tSample size\tTruth(bits)\tPlug-in(bits)\tPolynomial(bits)\n");
    while (k < 1000)
    {
        Entropy entropy( k );
        entropy.setDegree( c0*log(k) );
        entropy.setInterval( c1*log(k) );
        entropy.setThreshold( c2*log(k) );
        SampleGen gen;

        double truth = log(k)/log(2);
        int n = 50*k;
        double SE_plug = 0;
        double SE_poly = 0;
        for (int seed = 0; seed < trials; ++seed)
        {
            gen.reset();
            gen.setSeed( seed );
            gen.uniform( n, k );
            entropy.setFin( gen.getFin() );
            SE_plug += pow(truth-entropy.estimate_plug(),2);
            SE_poly += pow(truth-entropy.estimate(),2);
        }
        printf("%d\t\t%d\t\t%.6f\t%.6f\t%.6f\n", k, n, truth, sqrt(SE_plug/trials), sqrt(SE_poly/trials));
        k = floor(double(k)*1.1);
    }
    
    return;
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
    printf("Zipf(1) distribution.\nAlphabet size=%d.\nPolynoimal degree=%d.\nApproximation interval=[0,%.2f/n].\nPlug-in threshold=%d.\n",k,entropy.getDegree(),entropy.getInterval(), (int)floor(entropy.getThreshold())+1);
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
