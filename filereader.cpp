#include <iostream>
#include <iterator>
#include <set>
#include "filereader.h"
#include "stringop.h"

std::string FileReader::stdString(std::string word)
{
	StringOp::removepunc(word);
	StringOp::tolower(word);
	return word;
}

FileReader::const_HistPtr FileReader::read(int N)
{
	std::string word;
	for ( int i = 0; i < N;  ++i )
	{
		if ( (*mpFile) >> word )
		{
			word = stdString(word);
			addWord(word);
		}
		else
		{
			break;
		}
	}
	return mpHist;
}
void FileReader::addWord(std::string word)
{
	std::map<std::string,int>::iterator it = mpHist->find( word );
	if ( it == mpHist->end() )
		mpHist->insert( std::pair<std::string,int>( word,1 ) );
	else
		++(it->second);
}

int FileReader::wordN()
{
	int sum = 0;
	for ( std::map<std::string,int>::iterator it = mpHist->begin(); it != mpHist->end(); ++it )
		sum += it->second;
	return sum;
}


FileReader::const_HistPtr FileReader::hist()
{
	saveState();
	resetState();
	FileReader::HistPtr pCompleteHist ( new std::map<std::string, int> );
	std::istream_iterator<std::string> in(*mpFile), end;
	for (; in != end; ++in)
	{
		std::string word = stdString(*in);
		std::map<std::string,int>::iterator it = pCompleteHist->find( word );
		if ( it == pCompleteHist->end() )
			pCompleteHist->insert( std::pair<std::string,int>( word,1 ) );
		else
			++(it->second);
	}
	loadState();
	return pCompleteHist;
}

int FileReader::distinctTotal()
{
	saveState();
	resetState();
	std::istream_iterator<std::string> in(*mpFile), end;
	std::set<std::string> words;
	for (; in != end; ++in)
	{
		words.insert(stdString(*in));
	}
	loadState();
	return words.size();
}
int FileReader::wordTotal()
{
	saveState();
	resetState();
	std::istream_iterator<std::string> in(*mpFile), end;
	int count = std::distance(in, end);
	loadState();
	return count;
}
void FileReader::wordRepeat()
{
	saveState();
	resetState();
	std::istream_iterator<std::string> in(*mpFile), end;
	for (; in != end; ++in)
	{
		std::cout << *in<<" ";
	}
	std::cout << std::endl;
	loadState();
}



void FileReader::saveState()
{
	*mpState = mpFile->rdstate();
	*mpPos = mpFile->tellg();
}
void FileReader::resetState()
{
	mpFile->clear();
	mpFile->seekg(0);
}
void FileReader::loadState()
{
	mpFile->seekg(*mpPos);
	mpFile->setstate(*mpState);
}

FileReader::FileReader()
	:mpFile(new std::ifstream),
	 mpHist(new std::map<std::string, int>),
	 mpState(new std::ios::iostate),
	 mpPos(new std::streampos)
{}

FileReader::FileReader(std::string filename)
	:FileReader()
{
	mpFile->open( filename.c_str() );
}

FileReader::~FileReader()
{
	mpFile->close();
}

