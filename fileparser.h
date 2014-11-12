#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <string>
#include <fstream>
#include <map>
#include <memory>

class FileParser
{
public:
	typedef std::shared_ptr<std::map<std::string, int> >  HistPtr;
	typedef std::shared_ptr<const std::map<std::string, int> >  const_HistPtr;
	
    FileParser();
	FileParser(std::string filename);
    virtual ~FileParser();

	void clearHist(){ mpHist->clear(); }
	void reloadFile(){ resetState(); }
	const_HistPtr getHist() const{ return mpHist; }
	const_HistPtr hist(int cnt); // continue generating histogram of the next cnt words 
	int wordCounted(); // the number of words that have been counted
	int distinctWordCounted() { return mpHist->size(); }

	// Test the whole file
	const_HistPtr hist(); // histogram of all words
	int distinctWordCount(); // total number of distinct words
	int wordCount(); // total number of words
	void wordRepeat(); // repeart the file wordwise
	
private:
	std::ifstream* mpFile;
	HistPtr mpHist;

	virtual std::string stdString(std::string word); // standarderize the input word 
	
	// process the state inside the ifstream mpFile
	void saveState();
	void resetState();
	void loadState();
	std::unique_ptr<std::ios::iostate> mpState;
	std::unique_ptr<std::streampos> mpPos;
};



#endif
