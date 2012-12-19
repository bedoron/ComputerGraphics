#pragma once
#include "vec.h"
class Fog
{
	
public:
	vec3 fogColor;
	GLfloat fogDensity;
	Fog(vec3 color,GLfloat density):fogColor(color),fogDensity(density)
	{}
	vec3 getFogColor(){return fogColor/255.0;}
	GLfloat getFogDensity(){return fogDensity;}
	~Fog(void)
	{}
	Fog(Fog& f):fogColor(f.fogColor),fogDensity(f.fogDensity)
	{}
};

