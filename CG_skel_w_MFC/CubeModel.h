#pragma once
#include "scene.h"
class CubeModel :
	public Model
{
private:
	GLfloat _minX;
	GLfloat _maxX;
	GLfloat _minY;
	GLfloat _maxY;
	GLfloat _minZ;
	GLfloat _maxZ;
	mat4 _object_transform;
public:
	CubeModel(void);
	CubeModel(GLfloat minX,GLfloat maxX,GLfloat minY,GLfloat maxY,GLfloat minZ,GLfloat maxZ);
	~CubeModel(void);
	void draw(Renderer& renderer);
	void setObjectTransform(mat4 worldTransform);
	mat4 getObjectTransform();
	vec3 getModelCenter();
};

