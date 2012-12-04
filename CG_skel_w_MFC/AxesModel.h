#pragma once
#include "Scene.h"
class AxesModel : public Model
{
public:
	AxesModel(void);
	~AxesModel(void);
	void  draw(Renderer& renderer);
	void  setObjectTransform(mat4 worldTransform);
	mat4  getObjectTransform();
	vec3 getModelCenter();
};

