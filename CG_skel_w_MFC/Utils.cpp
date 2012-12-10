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
					Face newFace(objitem.getVertexByNumber(v1),objitem.getVertexByNumber(v2),objitem.getVertexByNumber(v3),vec3(v1,v2,v3));
					objitem.addFace(newFace);
				}
				else
				{
					Face newFace(objitem.getVertexByNumber(v1),objitem.getVertexByNumber(v2),objitem.getVertexByNumber(v3),vec3(v1,v2,v3)
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

GLfloat Utils::interpolateFace(Face face,GLfloat x, GLfloat y)
{
	vec3 _v1 = face.getVecX();
	vec3 _v2 = face.getVecY();
	vec3 _v3 = face.getVecZ();
	vec3 v1 = vec3(_v1.x,_v1.y,0);
	vec3 v2 = vec3(_v2.x,_v2.y,0);
	vec3 v3 = vec3(_v3.x,_v3.y,0);
	vec3 p(x,y,0);
	vec3 n = normalize(cross(v2-v1,v3-v1));
	GLfloat area = dot(n,cross(v2-v1,v3-v1));
	GLfloat a1 = dot(n,cross(v2-p,v3-p))/area;
	GLfloat a2 = dot(n,cross(v1-p,v3-p))/area;
	GLfloat a3 = 1 - a1 - a2;
	return dot(vec3(a1,a2,a3),vec3(_v1.z,_v2.z,_v3.z));
}

vec3 Utils::getBarycentricCoordinates(vec3 v1,vec3 v2,vec3 v3,vec3 point)
{
	vec3 n = normalize(cross(v2-v1,v3-v1));
	GLfloat area = dot(n,cross(v2-v1,v3-v1));
	GLfloat a1 = dot(n,cross(v1-point,v2-point))/area;
	GLfloat a2 = dot(n,cross(v2-point,v3-point))/area;
	GLfloat a3 = dot(n,cross(v3-point,v1-point))/area;
	return vec3(a1,a2,a3);
}


