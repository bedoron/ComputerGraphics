#pragma once
#include "vec.h"

struct Light {
public:
	vec4 position;
	vec4 color;

	struct sLightSource {
		vec4 position;
		vec4 color;
	};

	typedef  sLightSource LightSource;

	Light(vec4 p, vec4 c): position(p), color(c) { };

	operator LightSource() {
		LightSource src; //= { position, color };
		src.position = position;
		src.color = color;
		return src;
	};

	virtual ~Light() { };
};


//enum L_TYPE { L_AMBIANT=1600, L_PARALEL, L_POINT };
//class Light
//{
//
//private:
//	vec3 location;
//	vec3 direction;
//	vec3 intencity;
//	int lightType;
//	GLfloat _angle;
//public:
//	Light(void);
//	~Light(void);
//	void setDirection(vec3 d){direction=d;}
//	void setLocation(vec3 l){location=l;}
//	void setIntencity(vec3 i){intencity=i;}
//	vec3 getDirection(){return direction;}
//	vec3 getLocation(){return location;}
//	vec3 getIntencity(){return intencity;}
//	void setLightType(int type){lightType=type;}
//	void setAngle(GLfloat angle){_angle=angle;}
//	int getLightType() { return lightType;}
//	vec3 getLight(vec3& normal,vec3& _kAmbiant,vec3& _kDiffuze,vec3& _kSpecular,vec3& point,mat4& cam,Face& face,vec4& eye_at,GLfloat shininess)
//	{
//		vec3 v = vec3(eye_at.x,eye_at.y,eye_at.z);
//		vec3 tmpintensity = intencity/255;
//		vec3 light(0.0,0.0,0.0);
//		switch (lightType)
//		{
//		case L_AMBIANT:
//		{
//			light+=*_kAmbiant * tmpintensity;
//			break;
//		}
//		case L_PARALEL:
//		{
//			if(dot(normalize(normal),normalize(direction))>0)
//			{
//				GLfloat angle = dot(normalize(direction),normal);
//				light +=tmpintensity*_kDiffuze*angle;
//				vec3 h = normalize(v)+normalize(direction);
//				light += _kSpecular * max(pow(dot(normalize(h),normal),shininess),0);
//			}
//			break;
//		}
//		case L_POINT:
//		{
//			
//			//vec4 framelocation=location;
//			vec3 directionToPoint = point -location;
//			if(dot(normalize(normal),normalize(directionToPoint)>0))
//			{
//				GLfloat pointAngle = dot(normalize(direction),normalize(directionToPoint));
//				if(acos(pointAngle) < _angle*M_PI/180 && acos(pointAngle) > -_angle*M_PI/180)
//				{
//					light +=tmpintensity*dot(normalize(directionToPoint),normal)*_kDiffuze;
//					vec3 h = normalize(v)+normalize(direction);
//					light += _kSpecular * max(pow(dot(normalize(h),normal),shininess),0);
//				}
//			}
//		}
//		
//		}
//		light =vec3(max(light.x,0),max(light.y,0),max(light.z,0));
//		return light;
//	}
//};

