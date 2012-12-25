#pragma once
#include "vec.h"
#include <string>
class Material
{
private:
	vec3 _kambiant;
	vec3 _kdiffuze;
	vec3 _kspecular;
	GLfloat _ns;
	std::string name;
public:
	Material(std::string val);
	~Material(void);
	void setKambiant(vec3 val){_kambiant =val;}
	void setKdiffuse(vec3 val){_kdiffuze =val;}
	void setKspecular(vec3 val){_kspecular =val;}
	vec3 getKAmbiant(){return _kambiant;}
	vec3 getKDiffuze(){return _kdiffuze;}
	vec3 getKSpecular(){return _kspecular;}
	std::string getName(){return name;}
	void setNS(GLfloat val){_ns =val;}
	GLfloat getNS(){return _ns;}
};

