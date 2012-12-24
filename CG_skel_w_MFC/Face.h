#pragma once

#ifndef _FACE_
#define _FACE_
#include "mat.h"

class Renderer;

class Face 
{
private:
	vec3 _x; 
	vec3 _y; 
	vec3 _z; 
	vec3 _vn1;
	vec3 _vn2;
	vec3 _vn3;
	mat3 normalLine;
	vec3 _vertices;
	vec3 _normalVec;
	vec3 _kambiant;
	vec3 _kdiffuze;
	vec3 _kspecular;
	bool matrial;
public:
	Face(vec3 x,vec3 y,vec3 z,vec3 vertices=vec3(),vec3 vn1=vec3(0,0,0),vec3 vn2=vec3(0,0,0),vec3 vn3=vec3(0,0,0));

	~Face(void);
	vec3& getVecX();
	vec3& getVecY();
	vec3& getVecZ();
	vec3& getVnX();
	vec3& getVnY();
	vec3& getVnZ();
	void draw(Renderer& renderer);
	mat3 getNormalLine();
	vec3 getVertices();
	GLfloat getFaceArea();
	vec3 getNormal();
	void setVN1(vec3 vn){_vn1 = vn;}
	void setVN2(vec3 vn){_vn2 = vn;}
	void setVN3(vec3 vn){_vn3 = vn;}
	void setKambiant(vec3 val){matrial=false;_kambiant =val;}
	void setKdiffuse(vec3 val){matrial=false;_kdiffuze =val;}
	void setKspecular(vec3 val){matrial=false;_kspecular =val;}
	vec3 getKAmbiant(){return _kambiant;}
	vec3 getKDiffuze(){return _kdiffuze;}
	vec3 getKSpecular(){return _kspecular;}
	Face transformFace(Renderer& renderer);
	bool isMaterial(){return matrial;}
};

#endif // !_FACE_