#include "stdafx.h"
#include "AxesModel.h"


AxesModel::AxesModel(void)
{
}


AxesModel::~AxesModel(void)
{
}
void  AxesModel::draw(Renderer& renderer)
{
	renderer.drawLineByVectors(vec3(0,0,0), vec3(100,0,0), (unsigned int)RED);
	renderer.drawLineByVectors(vec3(0,0,0), vec3(0,100,0), (unsigned int)GREEN);
	renderer.drawLineByVectors(vec3(0,0,0), vec3(0,0,100), (unsigned int)BLUE);
}
void  AxesModel::setObjectTransform(mat4 worldTransform)
{
}

mat4  AxesModel::getObjectTransform()
{
	return mat4(1);
}
vec3 AxesModel::getModelCenter()
{
	return vec3(0,0,0);
}