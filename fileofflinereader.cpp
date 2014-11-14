#include "fileofflinereader.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <set>

std::string FileOfflineReader::stdString(std::string word)
{
	return word;
}

FileOfflineReader::const_HistPtr FileOfflineReader::randread(int N)
{
	for ( int i = 0; i < N;  ++i )
	{
		int loc = rand() % wordT;
		std::string word = stdString(words[loc]);
		addWord(word);
	}
	return mpHist;	
}

FileOfflineReader::const_HistPtr FileOfflineReader::read(int N)
{
	for ( int i = 0; i < N;  ++i )
	{
		if ( pos < wordT )
		{
			std::string word = stdString(words[pos]);
			addWord(word);
			++pos;
		}
		else
		{
			break;
		}
	}
	return mpHist;	
}
void FileOfflineReader::addWord(std::string word)
{
	std::map<std::string,int>::iterator it = mpHist->find( word );
	if ( it == mpHist->end() )
		mpHist->insert( std::pair<std::string,int>( word,1 ) );
	else
		++(it->second);
}

int FileOfflineReader::wordN()
{
	int sum = 0;
	for ( std::map<std::string,int>::iterator it = mpHist->begin(); it != mpHist->end(); ++it )
		sum += it->second;
	return sum;	
}


FileOfflineReader::FileOfflineReader()
	:mpHist(new std::map<std::string, int>)
{
}
FileOfflineReader::FileOfflineReader(std::string filename)
	:FileOfflineReader()
{
	std::ifstream infile;
	infile.open( filename.c_str() );

	std::istream_iterator<std::string> in(infile), end;
	wordT = std::distance(in, end);
	infile.clear();
	infile.seekg(0);

	words = boost::shared_array<std::string>(new std::string[wordT]);
	
	std::set<std::string> wordset;
	std::istream_iterator<std::string> in2(infile);
	for (int i = 0; i < wordT; ++i)
	{
		words[i] = *in2;
		wordset.insert(stdString(*in2));
		++in2;
	}
	distinctT = wordset.size();

	pos = 0;
	
	infile.close();
}
