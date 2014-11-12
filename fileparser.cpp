#include <iostream>
#include <iterator>
#include <set>
#include "fileparser.h"


std::string FileParser::stdString(std::string word)
{
	return word;
}



void FileParser::hist(int cnt)
{
	std::string word;
	for ( int i = 0; i < cnt;  ++i )
	{
		if ( (*m_file) >> word )
		{
			word = stdString(word);
			std::map<std::string,int>::iterator it = m_hist->find( word );
			if ( it == m_hist->end() )
				m_hist->insert( std::pair<std::string,int>( word,1 ) );
			else
				++(it->second);
		}
		else
		{
			break;
		}
	}
}

int FileParser::wordCounted()
{
	int sum = 0;
	for ( std::map<std::string,int>::iterator it = m_hist->begin(); it != m_hist->end(); ++it )
		sum += it->second;
	return sum;
}

std::map<std::string, int>* FileParser::hist()
{
	saveState();
	resetState();
	std::map<std::string, int>* t_hist = new std::map<std::string, int>;
	std::istream_iterator<std::string> in(*m_file), end;
	for (; in != end; ++in)
	{
		std::string word = stdString(*in);
		std::map<std::string,int>::iterator it = t_hist->find( word );
		if ( it == t_hist->end() )
			t_hist->insert( std::pair<std::string,int>( word,1 ) );
		else
			++(it->second);
	}
	loadState();
	return t_hist;
}

int FileParser::distinctWordCount()
{
	saveState();
	resetState();
	std::istream_iterator<std::string> in(*m_file), end;
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
	std::istream_iterator<std::string> in(*m_file), end;
	int count = std::distance(in, end);
	loadState();
	return count;
}
void FileParser::wordRepeat()
{
	saveState();
	resetState();
	std::istream_iterator<std::string> in(*m_file), end;
	for (; in != end; ++in)
	{
		std::cout << *in<<" ";
	}
	std::cout << std::endl;
	
	loadState();
}



void FileParser::saveState()
{
	*m_state = m_file->rdstate();
	*m_pos = m_file->tellg();
}
void FileParser::resetState()
{
	m_file->clear();
	m_file->seekg(0);
}
void FileParser::loadState()
{
	m_file->seekg(*m_pos);
	m_file->setstate(*m_state);
}


FileParser::FileParser(std::string filename)
{
	init();
	m_file->open( filename.c_str() );
}

FileParser::FileParser()
{
	init();
}
void FileParser::init()
{
	m_file = new std::ifstream;
	m_hist = new std::map<std::string, int>;
	m_state = new std::ios::iostate;
	m_pos= new std::streampos;
}
FileParser::~FileParser()
{
	m_file->close();
}


std::map<std::string, int>* FileParser::getHist()
{
	std::map<std::string, int>* t_hist = new std::map<std::string, int>(*m_hist); // a copy of m_hist
	return t_hist;
}
