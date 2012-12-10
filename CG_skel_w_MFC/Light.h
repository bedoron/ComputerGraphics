#pragma once
#include "mat.h"

class Light
{
private:
	vec3 location;
	vec3 direction;
	vec3 intencity;
	int lightType;
public:
	Light(void);
	~Light(void);
	void setDirection(vec3 d){direction=d;}
	void setLocation(vec3 l){location=l;}
	void setIntencity(vec3 i){intencity=i;}
	vec3 getDirection(){return direction;}
	vec3 getLocation(){return location;}
	vec3 getIntencity(){return intencity;}
};

