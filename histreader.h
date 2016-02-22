#ifndef HISTREADER_H
#define HISTREADER_H


#include <map>
#include <vector>

class HistReader
{
public:
    HistReader(){}
    virtual ~HistReader(){}

    void addHist(std::string filename);
    std::vector<int> getHist() const;             // return histogram
    
    int wordN() const;                            // the number of words read
    int distinctN() const { return hist.size(); } // the number of distinct words encountered
	
private:
    std::map<std::string, int> hist;
	
};


#endif
