#pragma once
#include "stdafx.h"

#include "OBJItem.h"
#include <string>
#include <exception>

using std::exception;
using std::string;

class ObjParserException: public exception { public:
	virtual const char* what() const throw() { return "ObjParserException: General exception"; }
};

class ObjParserFileNotFound: public ObjParserException { public:
	virtual const char* what() const throw() { return "ObjParserException: file not found"; }
};

class Utils 
{
	static Utils *instance;
	Utils();	
public:
	static Utils &getInstance();
	~Utils();
	OBJItem parseOBJ(string filename) throw(ObjParserException, ObjParserFileNotFound);
};

