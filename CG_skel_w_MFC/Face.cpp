#include "stdafx.h"
#include "Face.h"
#include "Renderer.h"

Face::Face(vec3 x,vec3 y,vec3 z,vec3 vn1,vec3 vn2,vec3 vn3):_x(x),_y(y),_z(z),normalLine(),_vn1(vn1),_vn2(vn2),_vn3(vn3)
{
	vec3 normal = normalize(cross(x-y,x-z)) ;
	vec3 center = (x+y+z)/3;
	normalLine[0]=center;
	normalLine[1] = center + normal;
}


Face::~Face(void)
{
}
vec3& Face::getVecX()
{
	return _x;
}
vec3& Face::getVecY()
{
	return _y;
}
vec3& Face::getVecZ()
{
	return _z;
}
vec3& Face::getVnX()
{
	return _vn1;
}
vec3& Face::getVnY()
{
	return _vn2;
}
vec3& Face::getVnZ()
{
	return _vn3;
}
mat3 Face::getNormalLine()
{
	return normalLine;
}

