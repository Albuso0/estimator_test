#include "entropy.h"
#include <iostream>
#include <algorithm>
#include "samplegen.h"
#include <vector>

#include "commandline.h"


typedef std::map<int, int> Fingerprint;
enum TEST { uniform, mix, zipf1, mixgeozipf };

void TEST_regime(TEST distr);
void TEST_fixed_P(TEST distr);
void TEST_fixed_P_RMSE(TEST distr);

int main(int argc, char *argv[])
{
    // std::CommandLine cmd;
    // cmd.AddValue ("seed",  "", seed);
    // cmd.Parse (argc, argv);

    // TEST_regime(zipf1);
    TEST_fixed_P(uniform);
    // A function that given input P, run simulation.
    
    return 0;
}




void TEST_fixed_P(TEST distr)
{
    // double c0 = 1.3, c1 = 8, c2 = 1.5;
    double c0 = 3, c1 = 3.5, c2 = 5;
    
    SampleGen gen;
    gen.reset();
    gen.setSeed( 0 );
    
    int k = 1000000;
    std::vector<double> p( k );
    double truth = 0;
    switch( distr )
    {
    case(uniform):
    {
        int supp = k/2;
        for (int i = 0; i < supp; ++i)
            p[i] = 1.0;
        truth = log(supp)/log(2);
        printf("Uniform distributed on half of alphabet.\n");
        break;
    }
    case(mix):
        for (int i = 0; i < k; i++)
            p[i] = 1;
        p[0] += k;
        truth = log(2*k)-(k+1)*log(k+1)/2/k;
        truth /= log(2);
        printf("Mixture by half uniform and half point mass.\n");
        break;
    case(zipf1):
    {
        double norm = 0.0;
        for (int i = 0; i < k; ++i)
        {
            p[i] = 1.0/(i+1);
            norm += p[i];
        }
        for (int i = 1; i <= k; ++i)
            truth += 1.0/double(i)/norm * log( double(i)*norm );
        truth /= log(2);
        printf("Zipf(1) distribution.\n");
        break;
    }
    case(mixgeozipf):
    {
        double norm1 = 0.0;
        int mid = floor(k/2);
        for (int i = 0; i < mid; ++i)
        {
            p[i] = 1.0/(i+1);
            norm1 += p[i];
        }
        for (int i = 0; i < mid; ++i)
            p[i] /= (2*norm1);
        double norm2 = 0;
        p[mid] = 2.0/k;
        norm2 += p[mid];
        for (int i = mid+1; i < k; ++i)
        {
            p[i] = p[i-1] * (1-2.0/k);
            norm2 += p[i];
        }
        for (int i = mid; i < k; ++i)
            p[i] /= (2*norm2);
        for (int i = 0; i < k; ++i)
            truth += ( -p[i]*log(p[i]) );
        truth /= log(2);
        printf("Half Zipf(1) distribution and Half Geo(2/k).\n");
        break;
    }
    }
    
    Entropy entropy( k );
    entropy.setDegree( c0*log(k) );
    entropy.setInterval( c1*log(k) );
    entropy.setThreshold( c2*log(k) );
    printf("Alphabet size=%d.\n", entropy.getAlphabetSize());
    printf("Polynoimal degree=%d.\n", entropy.getDegree());
    printf("Approximation interval=[0,%.2f/n].\n", entropy.getInterval());
    printf("Plug-in threshold=%d.\n",(int)floor(entropy.getThreshold())+1);
    printf("Unit: bits\n");
    printf("\nSample size\tTruth\t\tPlug-in\t\tMiller-Madow\tPolynomial\n");
    

    for ( int i = 1; i <= 50; ++i)
    {
        gen.discrete( 10000, &p );
        entropy.setFin( gen.getFin() );
        printf("%d\t\t%.6f\t%.6f\t%.6f\t%.6f\n", entropy.getSampleSize(), truth, entropy.estimate_plug(), entropy.estimate_Miller_Madow(), entropy.estimate() );
    }
    return;
}


void TEST_regime(TEST distr)
{
    double c0 = 1.3, c1 = 8, c2 = 1.5;
    int trials = 20;

    int k = 100;
    printf("Regime: sample size=50*support size.\n");
    printf("Unit: bits.\n");
    printf("\nSupport size\tSample size\tTruth\t\tRMSE:Plug\tRMSE:MM\t\tRMSE:Poly\n");

    while (k < 1000)
    {
        Entropy entropy( k );
        entropy.setDegree( c0*log(k) );
        entropy.setInterval( c1*log(k) );
        entropy.setThreshold( c2*log(k) );
        SampleGen gen;

        std::vector<double> p( k );
        double truth = 0;
        switch( distr )
        {
        case(uniform):
            truth = log(k)/log(2);
            for (int i = 0; i < k; ++i)
                p[i] = 1.0;
            break;
        case(zipf1):
            double norm = 0.0;
            for (int i = 0; i < k; ++i)
            {
                p[i] = 1.0/(i+1);
                norm += p[i];
            }
            for (int i = 1; i <= k; ++i)
                truth += 1.0/double(i)/norm * log( double(i)*norm );
            truth /= log(2);
            break;
        }
        
        int n = 50*k;
        double SE_plug = 0;
        double SE_poly = 0;
        double SE_MM = 0;
        for (int seed = 0; seed < trials; ++seed)
        {
            gen.reset();
            gen.setSeed( seed );
            gen.discrete( n, &p );
            entropy.setFin( gen.getFin() );
            // std::cout<<truth<<" "<<entropy.estimate_plug()<<" "<<entropy.estimate_Miller_Madow()<<std::endl;
            SE_plug += pow(truth-entropy.estimate_plug(),2);
            SE_MM += pow(truth-entropy.estimate_Miller_Madow(),2);
            SE_poly += pow(truth-entropy.estimate(),2);
        }
        printf("%d\t\t%d\t\t%.6f\t%.6f\t%.6f\t%.6f\n", k, n, truth, sqrt(SE_plug/trials), sqrt(SE_MM/trials), sqrt(SE_poly/trials));
        k = floor(double(k)*1.1);
    }
    
    return;
}

void TEST_fixed_P_RMSE(TEST distr)
{
    double c0 = 1.3, c1 = 8, c2 = 1.5;
    int trials = 20;
    
    SampleGen gen;
    
    int k = 1000000;
    std::vector<double> p( k );
    double truth = 0;
    switch( distr )
    {
    case(uniform):
    {
        int supp = k/2;
        for (int i = 0; i < supp; ++i)
            p[i] = 1.0;
        truth = log(supp)/log(2);
        printf("Uniform distributed on half of alphabet.\n");
        break;
    }
    case(mix):
        for (int i = 0; i < k; i++)
            p[i] = 1;
        p[0] += k;
        truth = log(2*k)-(k+1)*log(k+1)/2/k;
        truth /= log(2);
        printf("Mixture by half uniform and half point mass.\n");
        break;
    case(zipf1):
        double norm = 0.0;
        for (int i = 0; i < k; ++i)
        {
            p[i] = 1.0/(i+1);
            norm += p[i];
        }
        for (int i = 1; i <= k; ++i)
            truth += 1.0/double(i)/norm * log( double(i)*norm );
        truth /= log(2);
        printf("Zipf(1) distribution.\n");
        break;
    }
    
    Entropy entropy( k );
    entropy.setDegree( c0*log(k) );
    entropy.setInterval( c1*log(k) );
    entropy.setThreshold( c2*log(k) );
    printf("Alphabet size=%d.\n", k);
    printf("Polynoimal degree=%d.\n", entropy.getDegree());
    printf("Approximation interval=[0,%.2f/n].\n", entropy.getInterval());
    printf("Plug-in threshold=%d.\n",(int)floor(entropy.getThreshold())+1);
    printf("Number of trials=%d\n", trials);
    printf("Unit: bits\n");
    printf("\nSample size\tTruth\t\tRMSE:Plug\tRMSE:MM\t\tRMSE:Poly\n");
    

    for ( int i = 1; i <= 20; ++i)
    {
        double SE_plug = 0, SE_MM = 0, SE_poly = 0;

        for ( int seed = 0; seed < trials; ++seed )
        {
            gen.reset();
            gen.setSeed( seed );
            gen.discrete( 10000 * i, &p );
            entropy.setFin( gen.getFin() );
            SE_plug += pow(truth-entropy.estimate_plug(),2);
            SE_MM += pow(truth-entropy.estimate_Miller_Madow(),2);
            SE_poly += pow(truth-entropy.estimate(),2);
        }
        printf("%d\t\t%.6f\t%.6f\t%.6f\t%.6f\n", entropy.getSampleSize(), truth, sqrt(SE_plug/trials), sqrt(SE_MM/trials), sqrt(SE_poly/trials) );
    }
    return;
}
