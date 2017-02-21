#ifndef FINOP_H
#define FINOP_H

#include <vector>
#include <string>

typedef std::vector< std::pair<int, int> > vfin;
typedef std::vector<int> vint;


void addFin( vfin &fin, int freq, int cnt);
// set fingerprint
void setFin( vfin &fin, const vint &freq, const vint &cnt );
void setFin( vfin &fin, const std::string filename );
// set fingerprint through histogram
void setHist( vfin &fin, const vint &hist );
void setHist( vfin &fin, const std::string filename );

// access functions
int getSampleSize( const vfin &fin );



#endif

