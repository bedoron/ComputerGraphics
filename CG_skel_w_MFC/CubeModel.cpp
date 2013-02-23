#include "stdafx.h"
#include "CubeModel.h"


CubeModel::CubeModel(void)
{
}
CubeModel::CubeModel(GLfloat minX,GLfloat maxX,GLfloat minY,GLfloat maxY,GLfloat minZ,GLfloat maxZ):_minX(minX),
	_maxX(maxX),_minY(minY),_maxY(maxY),_minZ(minZ),_maxZ(maxZ)
{
}

CubeModel::~CubeModel(void)
{
}
void CubeModel::draw(Renderer& renderer)
{
	//renderer.SetObjectMatrices(_object_transform);
	//renderer.drawLineByVectors(vec3(_minX,_minY,_minZ),vec3(_maxX,_minY,_minZ),(unsigned int)RED);
	//renderer.drawLineByVectors(vec3(_minX,_minY,_minZ),vec3(_minX,_maxY,_minZ),(unsigned int)RED);
	//renderer.drawLineByVectors(vec3(_minX,_maxY,_minZ),vec3(_maxX,_maxY,_minZ),(unsigned int)RED);
	//renderer.drawLineByVectors(vec3(_maxX,_minY,_minZ),vec3(_maxX,_maxY,_minZ),(unsigned int)RED);
	//renderer.drawLineByVectors(vec3(_minX,_minY,_maxZ),vec3(_maxX,_minY,_maxZ),(unsigned int)RED);
	//renderer.drawLineByVectors(vec3(_minX,_minY,_maxZ),vec3(_minX,_maxY,_maxZ),(unsigned int)RED);
	//renderer.drawLineByVectors(vec3(_minX,_maxY,_maxZ),vec3(_maxX,_maxY,_maxZ),(unsigned int)RED);
	//renderer.drawLineByVectors(vec3(_maxX,_minY,_maxZ),vec3(_maxX,_maxY,_maxZ),(unsigned int)RED);
	//renderer.drawLineByVectors(vec3(_minX,_minY,_minZ),vec3(_minX,_minY,_maxZ),(unsigned int)RED);
	//renderer.drawLineByVectors(vec3(_maxX,_minY,_minZ),vec3(_maxX,_minY,_maxZ),(unsigned int)RED);
	//renderer.drawLineByVectors(vec3(_minX,_maxY,_minZ),vec3(_minX,_maxY,_maxZ),(unsigned int)RED);
	//renderer.drawLineByVectors(vec3(_maxX,_maxY,_minZ),vec3(_maxX,_maxY,_maxZ),(unsigned int)RED);
	throw string("Not implemented");
}
void CubeModel::setObjectTransform(mat4 worldTransform)
{
	_object_transform = worldTransform;
}
mat4 CubeModel::getObjectTransform()
{
	return _object_transform;
}

vec3 CubeModel::getModelCenter()
{
	vec4 center = _object_transform* vec3((_maxX + _minX ) / 2 , ( _maxY + _minY ) / 2 , ( _maxZ + _minZ ) / 2 );
	return vec3(center.x,center.y,center.z);
}
