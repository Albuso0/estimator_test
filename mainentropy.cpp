#include "entropy.h"
#include <iostream>
#include <algorithm>
#include "samplegen.h"
#include <vector>
#include "distvec.h"

#include "commandline.h"


typedef std::map<int, int> Fingerprint;

void TEST_fixed_P(std::vector<double> &p, Entropy &entropy, std::vector<int> &testN);
void TEST_fixed_P_RMSE(std::vector<double> &p, Entropy &entropy, std::vector<int> &testN, const int trials);


int main(int argc, char *argv[])
{
    // std::CommandLine cmd;
    // cmd.AddValue ("seed",  "", seed);
    // cmd.Parse (argc, argv);
    double c0 = 2, c1 = 3, c2 = 1;
    // double c0 = 1.3, c1 = 8, c2 = 1.5;
    int k = 100000;
    
    Entropy entropy( k );
    entropy.setDegree( c0*log(k) );
    entropy.setInterval( c1*log(k) );
    entropy.setThreshold( c2*log(k) );

    // Test sample size
    std::vector<int> testN;
    for ( int i = 1; i <= 50; ++i )
        testN.push_back( 1000*i );

    // Test distribution
    std::vector<double> p = mixgeozipf(k);

    // TEST_fixed_P(p, entropy, testN);

    const int trials = 20;
    TEST_fixed_P_RMSE(p, entropy, testN, trials);
    
    return 0;
}

void TEST_fixed_P(std::vector<double> &p, Entropy &entropy, std::vector<int> &testN)
{
    double truth = 0;
    double norm = 0;
    for (const auto &mass : p)
        norm += mass;
    for (const auto &mass : p)
        if (mass > 0)
            truth += ( -mass/norm*log(mass/norm) );
    truth /= log(2);
    
    printf("Alphabet size=%d.\n", entropy.getAlphabetSize());
    printf("Polynoimal degree=%d.\n", entropy.getDegree());
    printf("Approximation interval=[0,%.2f/n].\n", entropy.getInterval());
    printf("Plug-in threshold=%d.\n",(int)floor(entropy.getThreshold())+1);
    printf("Unit: bits\n");
    printf("\nSample size\tTruth\t\tPlug-in\t\tMiller-Madow\tPolynomial\n");

    SampleGen gen;
    gen.reset();
    gen.setSeed( 0 );

    int currentN = 0;
    for ( const auto &n : testN )
    {
        gen.discrete( n-currentN, &p );
        currentN = n;
        entropy.setFin( gen.getFin() );
        printf("%d\t\t%.6f\t%.6f\t%.6f\t%.6f\n", entropy.getSampleSize(), truth, entropy.estimate_plug(), entropy.estimate_Miller_Madow(), entropy.estimate() );
    }
    return;
}


void TEST_fixed_P_RMSE(std::vector<double> &p, Entropy &entropy, std::vector<int> &testN, const int trials)
{
    double truth = 0;
    double norm = 0;
    for (const auto &mass : p)
        norm += mass;
    for (const auto &mass : p)
        if (mass > 0)
            truth += ( -mass/norm*log(mass/norm) );
    truth /= log(2);
    
    printf("Alphabet size=%d.\n", entropy.getAlphabetSize());
    printf("Polynoimal degree=%d.\n", entropy.getDegree());
    printf("Approximation interval=[0,%.2f/n].\n", entropy.getInterval());
    printf("Plug-in threshold=%d.\n",(int)floor(entropy.getThreshold())+1);
    printf("Number of trials=%d\n", trials);
    printf("Unit: bits\n");
    printf("\nSample size\tTruth\t\tRMSE:plug\tRMSE:MM\t\tRMSE:Poly\n");

    SampleGen gen;
    for ( const auto &n : testN )
    {
        double SE_plug = 0, SE_MM = 0, SE_poly = 0;

        for ( int seed = 0; seed < trials; ++seed )
        {
            gen.reset();
            gen.setSeed( seed );
            gen.discrete( n, &p );
            entropy.setFin( gen.getFin() );
            SE_plug += pow(truth-entropy.estimate_plug(),2);
            SE_MM += pow(truth-entropy.estimate_Miller_Madow(),2);
            SE_poly += pow(truth-entropy.estimate(),2);
        }
        printf("%d\t\t%.6f\t%.6f\t%.6f\t%.6f\n", entropy.getSampleSize(), truth, sqrt(SE_plug/trials), sqrt(SE_MM/trials), sqrt(SE_poly/trials) );
    }

    // std::map< int, std::vector<double> > poly;
    // for ( const auto &n : testN )
    // {
    //     std::vector<double> temp;
    //     poly[n] = temp;
    // }
    // for ( int seed = 0; seed < trials; ++seed )
    // {
    //     gen.reset();
    //     gen.setSeed( seed );

    //     int currentN = 0;
    //     for ( const auto &n : testN )
    //     {
    //         gen.discrete( n-currentN, &p );
    //         currentN = n;
    //         entropy.setFin( gen.getFin() );
    //         poly[n].push_back(entropy.estimate());
    //     }
    // }

    // for ( const auto & results : poly )
    // {
    //     int samplesize = results.first;
    //     double SE_poly = 0.0;
    //     for ( const auto & estimate : results.second )
    //         SE_poly += pow(estimate-truth,2);
    //     printf("%d\t\t%.6f\t%.6f\n", results.first, truth, sqrt(SE_poly/trials) );
    // }
}    


// void TEST_regime(TEST distr)
// {
//     double c0 = 1.3, c1 = 8, c2 = 1.5;
//     int trials = 20;

//     int k = 100;
//     printf("Regime: sample size=50*support size.\n");
//     printf("Unit: bits.\n");
//     printf("\nSupport size\tSample size\tTruth\t\tRMSE:Plug\tRMSE:MM\t\tRMSE:Poly\n");

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
//                 p[i] = 1.0;
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
//         double SE_MM = 0;
//         for (int seed = 0; seed < trials; ++seed)
//         {
//             gen.reset();
//             gen.setSeed( seed );
//             gen.discrete( n, &p );
//             entropy.setFin( gen.getFin() );
//             // std::cout<<truth<<" "<<entropy.estimate_plug()<<" "<<entropy.estimate_Miller_Madow()<<std::endl;
//             SE_plug += pow(truth-entropy.estimate_plug(),2);
//             SE_MM += pow(truth-entropy.estimate_Miller_Madow(),2);
//             SE_poly += pow(truth-entropy.estimate(),2);
//         }
//         printf("%d\t\t%d\t\t%.6f\t%.6f\t%.6f\t%.6f\n", k, n, truth, sqrt(SE_plug/trials), sqrt(SE_MM/trials), sqrt(SE_poly/trials));
//         k = floor(double(k)*1.1);
//     }
    
//     return;
// }




