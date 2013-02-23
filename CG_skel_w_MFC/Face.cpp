#include "stdafx.h"
#include "Face.h"
#include "Renderer.h"

Face::Face(vec3 x,vec3 y,vec3 z,vec3 vertices,vec3 vn1,vec3 vn2,vec3 vn3,vec2 vt1,vec2 vt2,vec2 vt3):_x(x),_y(y),_z(z),_vertices(vertices) ,normalLine(),_vn1(vn1),_vn2(vn2),_vn3(vn3),matrial(true)
	,_kambiant(vec3(1)),_kdiffuze(vec3(1)),_kspecular(vec3(1)),_ns(25),_vt1(vt1),_vt2(vt2),_vt3(vt3)
{
	_normalVec= normalize(cross(x-y,x-z)) ;
	vec3 center = (x+y+z)/3;
	normalLine[0]=center;
	normalLine[1] = center + _normalVec;
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
vec2& Face::getVtX()
{
	if(_vt1.x!=0 && _vt1.y!=0)
		return _vt1;
	return vec2(abs(_x.x + _x.z/2) ,abs(_x.y + _x.z/2));
}
vec2& Face::getVtY()
{
	if(_vt2.x!=0 && _vt2.y!=0)
		return _vt2;
	return vec2(abs(_y.x + _y.z/2) ,abs(_y.y + _y.z/2));
}
vec2& Face::getVtZ()
{
	if(_vt3.x!=0 && _vt3.y!=0)
		return _vt3;
	return vec2(abs(_z.x + _z.z/2) ,abs(_z.y + _z.z/2));
}
mat3 Face::getNormalLine()
{
	return normalLine;
}

vec3 Face::getVertices()
{
	return _vertices;
}
GLfloat Face::getFaceArea()
{
	vec3 v1= _y -_x; 
	vec3 v2= _z -_x; 
	return abs(length(cross(v1,v2)));
}
vec3 Face::getNormal()
{
	return _normalVec;
}
//Face Face::transformFace(Renderer& renderer, bool mode)
//{
//	vec3 newX = renderer.calculateMvpPCTransformation(_x,mode);
//	vec3 newY = renderer.calculateMvpPCTransformation(_y,mode);
//	vec3 newZ = renderer.calculateMvpPCTransformation(_z,mode);
//	vec3 newvn1 = renderer.calculateMvpPCTransformation(_x+_vn1,mode);
//	vec3 newvn2 = renderer.calculateMvpPCTransformation(_y+_vn2,mode);
//	vec3 newvn3 = renderer.calculateMvpPCTransformation(_z+_vn3,mode);
//	return Face(newX,newY,newZ,_vertices,newvn1-newX,newvn2-newY,newvn3-newZ);
//}