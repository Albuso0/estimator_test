#include "stringop.h"
#include <algorithm>
#include <iostream>

void StringOp::toupper( std::string &str )
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}
void StringOp::tolower( std::string &str )
{
	std::transform( str.begin(), str.end(), str.begin(), ::tolower );
}
void StringOp::removepunc( std::string &str )
{
	str.erase (std::remove_if (str.begin (), str.end (), ispunct), str.end ());
}
