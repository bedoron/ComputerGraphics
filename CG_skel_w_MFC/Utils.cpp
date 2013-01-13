#include "stdafx.h";
#include "Utils.h"
#include "Material.h"
#include <vector>
#include "VNormal.h"
#include "OBJItem.h"
#include <stdlib.h>
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
void getMaterails(string mtlFile,vector<Material*>* materials)
{
	
	ifstream mtlStream;

	mtlStream.open(mtlFile);
	if(!mtlStream.is_open())
		throw ObjParserFileNotFound();
	string fileLine;
	Material* material = NULL;
	
	while(getline(mtlStream, fileLine)) 
	{
		string command;
		istringstream mtlIsStream(fileLine);
		mtlIsStream >> command;
	
		if(command=="newmtl")
		{
			if(material)
				materials->push_back(material);
			string name;
			mtlIsStream >> name;
			material = new Material(name);
		}
		else if(command == "Ka")
		{
			GLfloat r=0,g=0,b=0;
			mtlIsStream >> r >> g >> b;
			material->setKambiant(vec3(b,g,r));
		}
		else if(command == "Kd")
		{
			GLfloat r=0,g=0,b=0;
			mtlIsStream >> r >> g >> b;
			material->setKdiffuse(vec3(b,g,r));
		}
		else if(command == "Ks")
		{
			GLfloat r=0,g=0,b=0;
			mtlIsStream >> r >> g >> b;
			material->setKspecular(vec3(b,g,r));
		}
		else if(command == "Ns")
		{
			GLfloat ns;
			mtlIsStream >> ns;
			material->setNS(ns);
		}


	}
	if(material)
		materials->push_back(material);
}
OBJItem Utils::parseOBJ(string filename,string fileID) {
	OBJItem objitem;
	ifstream objfile;

	objfile.open(filename);
	
	if(!objfile.is_open())
		throw ObjParserFileNotFound();

	string fileLine;
	vec3 zero(0,0,0);
	objitem.addVertex(zero);
	objitem.addNormal(zero);
	Material * currentMaterial=NULL;
	vector<Material*> materials;
	while(getline(objfile, fileLine)) {
		if(fileLine==""||fileLine[0]=='#')
			continue;
		istringstream line(fileLine);
		string action;
		line >> action;
		if(action == "mtllib")
		{
			string mtlfile,command;

			istringstream mtlline(fileLine);
			mtlline >> command >> mtlfile;
			char drive[5];
			char filename1[20];
			char path[200];
			char ext[5];
			_splitpath(filename.c_str(),drive,path,filename1,ext);
			string mtlFilePath = string(drive) + string(path) + mtlfile;
			ifstream ifile(mtlFilePath);
			if (ifile) 
				getMaterails(mtlFilePath,&materials);

		}
		else if(action == "usemtl")
		{
			string matName;
			line >> matName;
			for(vector<Material*>::iterator it = materials.begin();it!= materials.end(); ++ it)
			{
				if((*it)->getName() == matName)
					currentMaterial = (*it);
			}
		}
		else if(action == "v") {// vertex
			float x,y,z = 0;
			line >> x >> y >> z;
			vec3 newPoint(x,y,z);
			objitem.addVertex(newPoint);
		}
		else if(action == "f") {
			int vn1 = 0 , vn2 = 0 , vn3 = 0 , vn4 = 0;
			int vt1 = 0 , vt2 = 0 , vt3 = 0 , vt4 = 0;
			int v1 = 0 , v2 = 0 , v3 = 0 , v4 = 0;
			int size=objitem.getVertexSize();
			if(std::string::npos != fileLine.find("/"))
			{
				if(std::string::npos != fileLine.find("//"))
					sscanf(fileLine.c_str(),"f %d//%d %d//%d %d//%d %d//%d",&v1,&vn1,&v2,&vn2,&v3,&vn3,&v4,&vn4);
				else
					sscanf(fileLine.c_str(),"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",&v1,&vt1,&vn1,&v2,&vt2,&vn2,&v3,&vt3,&vn3,&v4,&vt4,&vn4);
				if(v2==0 && v3==0)
					sscanf(fileLine.c_str(),"f %d/%d %d/%d %d/%d %d/%d",&v1,&vt1,&v2,&vt2,&v3,&vt3,&v4,&vt4);
				
			}
			else
				sscanf(fileLine.c_str(),"f %d %d %d %d",&v1,&v2,&v3,&v4);

			try
			{
				if(v1 < 0)
					v1 = objitem.getVertexSize()+v1;
				if(v2 < 0)
					v2 = objitem.getVertexSize()+v2;
				if(v3 < 0)
					v3 = objitem.getVertexSize()+v3;
				if(vn1 < 0)
					vn1 = objitem.getVNsize()+vn1;
				if(vn2 < 0)
					vn2 = objitem.getVNsize()+vn2;
				if(vn3 < 0)
					vn3 = objitem.getVNsize()+vn3;
				
				if(vn1==0 &&vn2==0 && vn3==0)
				{
					Face newFace(objitem.getVertexByNumber(v1),objitem.getVertexByNumber(v2),objitem.getVertexByNumber(v3),vec3(v1,v2,v3));
					if(currentMaterial)
					{
						newFace.setKambiant(currentMaterial->getKAmbiant());
						newFace.setKdiffuse(currentMaterial->getKDiffuze());
						newFace.setKspecular(currentMaterial->getKSpecular());
						newFace.setNS(currentMaterial->getNS());
					}
					objitem.addFace(newFace);
					if(v4 !=0 || vn4 != 0 || vt4 !=0)
					{
						Face newFace2(objitem.getVertexByNumber(v1),objitem.getVertexByNumber(v3),objitem.getVertexByNumber(v4),vec3(v1,v3,v4));
						if(currentMaterial)
						{
							newFace2.setKambiant(currentMaterial->getKAmbiant());
							newFace2.setKdiffuse(currentMaterial->getKDiffuze());
							newFace2.setKspecular(currentMaterial->getKSpecular());
							newFace.setNS(currentMaterial->getNS());
						}
						objitem.addFace(newFace2);
					}

				}
				else
				{
					Face newFace(objitem.getVertexByNumber(v1),objitem.getVertexByNumber(v2),objitem.getVertexByNumber(v3),vec3(v1,v2,v3)
						,objitem.getNormalByNumber(vn1),objitem.getNormalByNumber(vn2),objitem.getNormalByNumber(vn3));
					if(currentMaterial)
					{
						newFace.setKambiant(currentMaterial->getKAmbiant());
						newFace.setKdiffuse(currentMaterial->getKDiffuze());
						newFace.setKspecular(currentMaterial->getKSpecular());
					}
					objitem.addFace(newFace);
					if(v4 !=0 || vn4 != 0 || vt4 !=0)
					{
						Face newFace2(objitem.getVertexByNumber(v1),objitem.getVertexByNumber(v3),objitem.getVertexByNumber(v4),vec3(v1,v3,v4)
							,objitem.getNormalByNumber(vn1),objitem.getNormalByNumber(vn3),objitem.getNormalByNumber(vn4));
						if(currentMaterial)
						{
							newFace2.setKambiant(currentMaterial->getKAmbiant());
							newFace2.setKdiffuse(currentMaterial->getKDiffuze());
							newFace2.setKspecular(currentMaterial->getKSpecular());
						}
						objitem.addFace(newFace2);
					}
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
	objitem.copyData();
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
vec3 Utils::getBarycentricCoordinates(Face face,GLfloat x, GLfloat y, GLfloat z)
{
	vec3 v1 = face.getVecX();
	vec3 v2 = face.getVecY();
	vec3 v3 = face.getVecZ();
	vec3 point(x,y,z);
	vec3 n = normalize(cross(v2-v1,v3-v1));
	GLfloat area = dot(n,cross(v2-v1,v3-v1));
	GLfloat a1 = dot(n,cross(v1-point,v2-point))/area;
	GLfloat a2 = dot(n,cross(v2-point,v3-point))/area;
	GLfloat a3 = dot(n,cross(v3-point,v1-point))/area;
	return vec3(a1,a2,a3);

}
