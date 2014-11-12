#include <iostream>
#include <iterator>
#include <set>
#include "fileparser.h"

std::string FileParser::stdString(std::string word)
{
	return word;
}

FileParser::const_HistPtr FileParser::incHist(int cnt)
{
	std::string word;
	for ( int i = 0; i < cnt;  ++i )
	{
		if ( (*mpFile) >> word )
		{
			word = stdString(word);
			std::map<std::string,int>::iterator it = mpHist->find( word );
			if ( it == mpHist->end() )
				mpHist->insert( std::pair<std::string,int>( word,1 ) );
			else
				++(it->second);
		}
		else
		{
			break;
		}
	}
	return mpHist;
}

int FileParser::wordCounted()
{
	int sum = 0;
	for ( std::map<std::string,int>::iterator it = mpHist->begin(); it != mpHist->end(); ++it )
		sum += it->second;
	return sum;
}


FileParser::const_HistPtr FileParser::hist()
{
	saveState();
	resetState();
	FileParser::HistPtr pCompleteHist ( new std::map<std::string, int> );
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

int FileParser::distinctWordCount()
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
int FileParser::wordCount()
{
	saveState();
	resetState();
	std::istream_iterator<std::string> in(*mpFile), end;
	int count = std::distance(in, end);
	loadState();
	return count;
}
void FileParser::wordRepeat()
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



void FileParser::saveState()
{
	*mpState = mpFile->rdstate();
	*mpPos = mpFile->tellg();
}
void FileParser::resetState()
{
	mpFile->clear();
	mpFile->seekg(0);
}
void FileParser::loadState()
{
	mpFile->seekg(*mpPos);
	mpFile->setstate(*mpState);
}

FileParser::FileParser()
	:mpFile(new std::ifstream),
	 mpHist(new std::map<std::string, int>),
	 mpState(new std::ios::iostate),
	 mpPos(new std::streampos)
{}

FileParser::FileParser(std::string filename)
	:FileParser()
{
	mpFile->open( filename.c_str() );
}

FileParser::~FileParser()
{
	mpFile->close();
}

