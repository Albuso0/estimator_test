#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <fstream>
#include <map>
#include <memory>

// FileReader can read a file can generate histogram of words in the file.
// This is an online reader, we do not require memory to store all huge file.
class FileReader
{
public:
	typedef std::shared_ptr<std::map<std::string, int> >  HistPtr;
	typedef std::shared_ptr<const std::map<std::string, int> >  const_HistPtr;
	
    FileReader();
	FileReader(std::string filename);
    virtual ~FileReader();

	void reset(){ resetHist(); resetFile(); }
	const_HistPtr getHist() const{ return mpHist; }
	const_HistPtr read(int N); // read the next N words in the file, return the pointer to updated histogram.
	int wordN(); // the number of words that have been read
	int distinctN() { return mpHist->size(); }

	int distinctTotal(); // total number of distinct words
	int wordTotal(); // total number of words
protected:
	void resetHist(){ mpHist->clear(); }
	void resetFile(){ resetState(); }
	// Test the whole file
	const_HistPtr hist(); // histogram of all words
	void wordRepeat(); // repeart the file wordwise
	
private:
	std::ifstream* mpFile;
	HistPtr mpHist;
	void addWord(std::string word);

	std::string stdString(std::string word); // standarderize the input word 
	
	// process the state inside the ifstream mpFile
	// BUG: those internal steps does not work!!!!
	void saveState();
	void resetState();
	void loadState();
	std::unique_ptr<std::ios::iostate> mpState;
	std::unique_ptr<std::streampos> mpPos;
};



#endif
