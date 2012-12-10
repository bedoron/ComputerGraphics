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

};

#endif // !_FACE_