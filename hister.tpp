#include <fstream>
#include <iostream>

template <typename T>
std::vector<int> Hister<T>::getHist() const
{
    std::vector<int> out_hist;
    for ( const auto & pair : hist )
        out_hist.push_back( pair.second );
    return out_hist;
}


template <typename T>
void Hister<T>::printHist() const
{
    for ( const auto & pair : hist )
        std::cout<<pair.first<<"\t"<<pair.second<<std::endl;
}



template <typename T>
std::size_t Hister<T>::samplesN() const
{
    std::size_t sum = 0;
    for ( const auto & pair : hist )
        sum += pair.second;
    return sum;	
}

template <typename T>
void Hister<T>::addSample(T sample)
{
    auto it = hist.find( sample );
    if ( it == hist.end() )
        hist.insert( std::pair<T,int>( sample,1 ) );
    else
        ++(it->second);
}


template <typename T>
void Hister<T>::addSampleFile(std::string filename)
{
    std::ifstream infile;
    infile.open( filename.c_str() );

    T Sample;
    while ( (infile >> Sample).good() )
    {
        add(Sample);
    }
    infile.close();
}


template <typename T>
void Hister<T>::addHist(T data, int freq)
{
    auto it = hist.find( data );
    if ( it == hist.end() )
        hist.insert( std::make_pair( data ,freq ) );
    else
        it->second += freq;
}

template <typename T>
void Hister<T>::addHistFile(std::string filename)
{
    std::ifstream infile;
    infile.open( filename.c_str() );

    T data;
    int freq;
    while ( (infile >> data).good() )
    {
        infile >> freq;
        addHist(data,freq);
    }
    infile.close();
}
