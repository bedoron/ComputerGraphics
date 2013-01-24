#pragma once
#include "gl/glew.h"
#include "Utils.h";
#include <string>
using std::string;
class MeshModel;
class Camera 
{
	vec3 m_position;
	mat4 cInverse;
	mat4 cTransform;
	mat4 projection;
	mat3 transform; // the clean transform not in 4D
	vec4 _eye;
	vec4 _at;
	vec4 _up;
	OBJItem arrow;
	string name;
public:
	Camera(const vec3 &position);
	Camera();
	~Camera();

	mat4 getInverseTransformation() { return cInverse; }
	mat4 getTransformation() {return cTransform; };
	mat4 getProjection() { return projection; };
	void setInverseMat(mat4 inverse);
	void setTransformation(const mat4& transform);
	mat4 LookAt(const vec4& eye, const vec4& at, const vec4& up );
	mat4 Ortho( const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar );
	// These two are the same
	void Frustum( const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar );
	void Perspective( const float fovy, const float aspect,
		const float zNear, const float zFar);
	void pointCameraAt(vec4 at);
	const string& getName();
	void draw(Renderer& renderer);
	void setProjection(mat4 _projection) { projection = _projection;}
	vec4 getEye(){return _eye;}
	vec4 getAt(){return _at;}
	vec4 getUp(){return _up;}
};
