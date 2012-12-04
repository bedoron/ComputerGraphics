#include "stdafx.h"
#include "Vertex.h"


Vertex::Vertex(float x,float y,float z):_x(x),_y(y),_z(z)
{
}
Vertex::~Vertex(void)
{
}
GLfloat Vertex::getX()
{
	return _x;
}
GLfloat Vertex::getY()
{
	return _y;
}
GLfloat Vertex::getZ()
{
	return _z;
}