#pragma once
#include <vector>
#include "GL\glew.h"
#include "vec.h"
#include "Light.h"

using std::vector;
typedef Light::LightSource LightSource;

class LightsUBO
{
	GLuint			_bufferHandle;
	vector<Light*>	lights;
	vec4			globalAmbient;

	struct sLightSourcesBlock  {
	vec4		globalAmbient;
	LightSource lightSources[12];
	int		num_lights;
	};
	
	typedef struct sLightSourcesBlock LightSourcesBlock;

public:
	LightsUBO(void);
	~LightsUBO(void);

	LightsUBO& operator<<(vector<Light*>);
	void setGlobalAmbient(vec4 ambient);
	vec4 getGlobalAmbient();
	void bind();
	void unbind();
	void bindToPoint(GLuint point);
};

