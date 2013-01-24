#pragma once

#include "gl/glew.h"
#include <vector>
#include <string>
//#include "Renderer.h"
#include "Utils.h"
#include "Camera.h"
#include "Light.h"
using namespace std;
enum ActiveEntity_t { WORLD_ACTIVE, MODEL_ACTIVE, CAMERA_ACTIVE };
class CModelData;

class Model {
protected:
	string name;
	virtual ~Model() {}
public:
	void virtual reDraw(GLuint program)=0;
	void virtual drawNormal(GLuint program)=0;
	void virtual setObjectTransform(mat4 worldTransform)=0;
	mat4 virtual getObjectTransform()=0;
	vec3 virtual getModelCenter()=0;
	const string& getName() const { return name; };
	const string& setName(const string &newName) { name = newName; return name; };
};

//
typedef vec4  color4;
typedef vec4  point4;
enum projection{ortho,prespctive,frustum};
class Scene {
	point4 eye;
	point4 at;
	point4 up;
	mat4 _current_Projection;
	mat4 _currentCamera;
	vector<Model*> models;
	GLuint program;
	MeshModel* _activeModel;
	projection _currentPtype;
	GLfloat _left,_right,_top,_bottom,_zfar,_znear,_fovy,_aspect;
public:
	Scene();
	void LookAt( const vec4& eye, const vec4& at, const vec4& up )
	{
		vec4 zeroVec(0,0,0,0);
		vec4 tempVec = at-eye;
		if ((tempVec) == zeroVec)
		{
			_currentCamera=  mat4(1.0);
			return;
		}
		vec3 eye3(eye.x,eye.y ,eye.z);
		vec3 at3 (at.x , at.y , at.z);
		vec3 up3 (up.x , up.y , up.z);
		vec3 n = normalize(eye3-at3);   //z'
		vec3 u = normalize(cross(up3,n));  //x'

		vec3 v = normalize(cross(n,u));  //y'

		//vec4 t = vec4(0.0,0.0,0.0,1.0);
		mat3 c = transpose (mat3(u,v,n)) ;
		_currentCamera= mat4(c) * Translate(-eye);
		this->eye = eye;

	}

	void initDefaultCamera();
	void initDefaultLight();
	void loadOBJModel(MeshModel* model);

	void setFrustum(float left,float right,float top,float down,float zNear,float zFar);
	void setOrtho(float left,float right,float top,float down,float zNear,float zFar);
	void setPrespective(float fovy,float aspect,float znear, float zfar);
	void setZoom(GLfloat zoom);
	void transformActiveModel(mat4 translation);
	void draw();
	void redraw();
	
};