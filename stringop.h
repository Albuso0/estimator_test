#ifndef STRINGOP_H
#define STRINGOP_H
#include <string>

class StringOp
{
public:
    StringOp(){}
    virtual ~StringOp(){}
	static void toupper(std::string &str);
	static void tolower(std::string &str);
	static void removepunc(std::string &str);
};



#endif
