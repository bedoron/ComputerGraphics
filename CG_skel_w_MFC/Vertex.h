#pragma once
#include "vec.h"
class Vertex
{
private:
	GLfloat _x,_y,_z;
public:
	Vertex(float x,float y,float z);
	~Vertex(void);
	float getX();
	float getY();
	float getZ();
};

