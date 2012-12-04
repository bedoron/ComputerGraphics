#include "stdafx.h";
#include "Utils.h"

#include "VNormal.h"
#include "OBJItem.h"

#include<iostream>
#include<fstream>
#include <sstream>
#include <string>

using std::string;
using std::ifstream;
using std::istringstream;



Utils::Utils() {
}

Utils::~Utils() {
	delete instance;
}

Utils *Utils::instance = 0;

Utils &Utils::getInstance() {
	if(instance == 0)
		instance = new Utils;
	return *instance;
}

OBJItem Utils::parseOBJ(string filename) {
	OBJItem objitem;
	ifstream objfile;
	
	objfile.open(filename);

	if(!objfile.is_open())
		throw ObjParserFileNotFound();

	string fileLine;
	vec3 zero(0,0,0);
	objitem.addVertex(zero);
	while(getline(objfile, fileLine)) {
		if(fileLine==""||fileLine[0]=='#')
			continue;
		istringstream line(fileLine);
		string action;
		line >> action;
		if(action == "v") {// vertex
			float x,y,z = 0;
			line >> x >> y >> z;
			vec3 newPoint(x,y,z);
			objitem.addVertex(newPoint);
		}
		else if(action == "f") {
			int vn1 = 0 , vn2 = 0 , vn3 = 0;
			int vt1 = 0 , vt2 = 0 , vt3 = 0;
			int v1 = 0 , v2 = 0 , v3 = 0;
			int size=objitem.getVertexSize();
			if(std::string::npos != fileLine.find("/"))
			{
				if(std::string::npos != fileLine.find("//"))
					sscanf(fileLine.c_str(),"f %d//%d %d//%d %d//%d",&v1,&vn1,&v2,&vn2,&v3,&vn3);
				else
					sscanf(fileLine.c_str(),"f %d/%d/%d %d/%d/%d %d/%d/%d",&v1,&vt1,&vn1,&v2,&vt2,&vn2,&v3,&vt3,&vn3);
			}
			else
				sscanf(fileLine.c_str(),"f %d %d %d",&v1,&v2,&v3);
			try
			{
				if(vn1==0 &&vn2==0 && vn3==0)
				{
					Face newFace(objitem.getVertexByNumber(v1),objitem.getVertexByNumber(v2),objitem.getVertexByNumber(v3));
					objitem.addFace(newFace);
				}
				else
				{
					Face newFace(objitem.getVertexByNumber(v1),objitem.getVertexByNumber(v2),objitem.getVertexByNumber(v3)
						,objitem.getNormalByNumber(vn1),objitem.getNormalByNumber(vn2),objitem.getNormalByNumber(vn3));
					objitem.addFace(newFace);
				}
			}
			catch(exception& e)
			{
				e.what();
			}
			
		
		}
		else if(action == "vn") {
			float i,j,k = 0;
			line >> i >> j >> k;
			vec3 normal(i,j,k);
			objitem.addNormal(normal);
		}
	}
	return objitem;
}







