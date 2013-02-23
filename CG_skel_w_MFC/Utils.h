#pragma once
#include "stdafx.h"

#include "OBJItem.h"
#include <string>
#include <exception>
#define noiseWidth 512
#define noiseHeight 512
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
	double noise[noiseWidth][noiseHeight];
public:
	static Utils &getInstance();
	~Utils();
	OBJItem parseOBJ(string filename,string fileID) throw(ObjParserException, ObjParserFileNotFound);
	static GLfloat interpolateFace(Face face,GLfloat x, GLfloat y);
	vec3 getBarycentricCoordinates(Face face,GLfloat x, GLfloat y, GLfloat z);
	void generateNoise();
	double smoothNoise(double x, double y);
	double turbulence(double x, double y, double size);
	GLubyte* marble();
	GLubyte* wood();
};

