#ifndef FILEOFFLINEREADER_H
#define FILEOFFLINEREADER_H

#include <string>
#include <map>
#include <memory>
#include <boost/shared_array.hpp>
#include <vector>


// FileOfflineReader can read a file can generate histogram of words in the file.
// This is an offline reader, we store the whole file when loading, thus is not suitable for huge files.
class FileOfflineReader
{
public:
	typedef std::shared_ptr<std::map<std::string, int> >  HistPtr;
	typedef std::shared_ptr<const std::map<std::string, int> >  const_HistPtr;
	
    FileOfflineReader();
	FileOfflineReader(std::string filename);
    virtual ~FileOfflineReader(){}

	void reset(){ resetPos(); resetHist(); }
	void setPos( int _pos ){ pos = _pos; }
	const_HistPtr getHist() const{ return mpHist; }
	const_HistPtr randread( int N ); // randomly read N words in the file, return the pointer to updated histogram.
	const_HistPtr read( int N ); // read the next N words in the file, return the pointer to updated histogram.
	const_HistPtr read( const std::vector<int> &PosVec ); // read words from position vector
	int wordN(); // the number of words that have been read
	int distinctN() { return mpHist->size(); }

	int distinctTotal(){ return distinctT; } // total number of distinct words
	int wordTotal(){ return wordT; } // total number of words
protected:
	void resetPos() { pos = 0; }
	void resetHist(){ mpHist->clear(); }
	
private:
	HistPtr mpHist;
	boost::shared_array<std::string> words;
	int pos;
	int wordT;
	int distinctT;
	void addWord(std::string word);

	std::string stdString(std::string word); // standarderize the input word 
	
};



#endif
