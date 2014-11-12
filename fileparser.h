#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <string>
#include <fstream>
#include <map>

class FileParser
{
public:
    FileParser();
	FileParser(std::string filename);
    virtual ~FileParser();

	void clearHist(){ m_hist->clear(); }
	void reloadFile(){ resetState(); }
	std::map<std::string, int>* getHist(); // get a copy of the current histogram
	void hist(int cnt); // continue generating histogram of the next cnt words 
	int wordCounted(); // the number of words that have been counted

	// Test the whole file
	std::map<std::string, int>* hist(); // histogram of all words 
	int distinctWordCount(); // total number of distinct words
	int wordCount(); // total number of words
	void wordRepeat(); // repeart the file wordwise
	
private:
	std::ifstream* m_file;
	std::map<std::string, int>* m_hist;

	virtual std::string stdString(std::string word); // standarderize the input word 
	
	// process the state inside m_file
	void saveState();
	void resetState();
	void loadState();
	std::ios::iostate* m_state;
	std::streampos* m_pos;
	// initialize the varibles
	void init();
};



#endif
