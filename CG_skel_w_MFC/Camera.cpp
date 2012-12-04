#include "stdafx.h"
#include "Camera.h"
#include <strstream>
using std::stringstream;

Camera::Camera(const vec3 &position): m_position(position)
{

	
}

Camera::Camera(): m_position(vec3(0,0,0)) 
{
	
}

Camera::~Camera(void) {
}
void Camera::pointCameraAt(vec4 at)
{
	LookAt(_eye,at,_up);
}
mat4 Camera::LookAt(const vec4& eye, const vec4& at, const vec4& up ) 
{
	_eye = eye;
	_at = at;
	_up = up;
	vec4 zeroVec(0,0,0,0);
	vec4 tempVec = at-eye;
	if ((tempVec) == zeroVec)
	{
		return mat4(1);
	}
	vec3 eye3(eye.x,eye.y ,eye.z);
	vec3 at3 (at.x , at.y , at.z);
	vec3 up3 (up.x , up.y , up.z);
	vec3 n = normalize(eye3-at3);   //z'
	vec3 u = normalize(cross(up3,n));  //x'

	vec3 v = normalize(cross(n,u));  //y'
	
	//vec4 t = vec4(0.0,0.0,0.0,1.0);
	mat3 c = transpose (mat3(u,v,n)) ;
	cInverse = mat4(c) * Translate(-eye);

	return cTransform;

}

void Camera::setTransformation(const mat4& transform) {
	cTransform = transform;
}

void Camera::setInverseMat(mat4 inverse)
{
	cInverse = inverse;
}
void Camera::Ortho( const float left, const float right, const float bottom, const float top, const float zNear, const float zFar ) {
	mat4 p;
	p[0][0] = 2/(right-left); p[0][1] = 0				; p[0][2] = 0				; p[0][3] = -(right + left)/(right - left);
	p[1][0] = 0				; p[1][1] = 2/(top-bottom)	; p[1][2] = 0				; p[1][3] = -(top + bottom)/(top- bottom);
	p[2][0] = 0				; p[2][1] = 0				; p[2][2] = 2/(zNear - zFar); p[2][3] = -(zNear + zFar)/(zNear- zFar);
	p[3][0] = 0				; p[3][1] = 0				; p[3][2] = 0				; p[3][3] = 1;
	projection = p;
}
void Camera::Frustum( const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar )
{
	mat4 p;
	p[0][0] = 2*zNear/(right-left)	; p[0][1] = 0						; p[0][2] =  (right + left)/(right - left);	; p[0][3] = 0;
	p[1][0] = 0						; p[1][1] = 2*zNear/(top-bottom)	; p[1][2] = (top + bottom)/(top - bottom)	; p[1][3] = 0;
	p[2][0] = 0						; p[2][1] = 0						; p[2][2] = -(zFar + zNear)/(zFar - zNear)	; p[2][3] = -2*zNear*zFar/(zFar-zNear);
	p[3][0] = 0						; p[3][1] = 0						; p[3][2] = -1								; p[3][3] = 0;
	projection = p;
}

void Camera::Perspective( const float fovy, const float aspect,
		const float zNear, const float zFar)
{
	GLfloat top = zNear*tan(M_PI* fovy/180);
	GLfloat right = top*aspect;
	mat4 c;
	c[0][0] = zNear/right;		c[0][1] = 0;				c[0][2] = 0;						c[0][3] = 0;
	c[1][0] = 0;				c[1][1] = zNear/top;		c[1][2] = 0;						c[1][3] = 0; 
	c[2][0] = 0;				c[2][1] = 0;				c[2][2] = (-zFar+zNear)/(zFar-zNear);c[2][3] = (-2*zFar*zNear)/(zFar-zNear); 
	c[3][0] = 0;				c[3][1] = 0;				c[3][2] = -1;						c[3][3] = 0; 

	projection = c;
}
void Camera::draw(Renderer& renderer)
{
	vec3 eye3(_eye.x,_eye.y,_eye.z);
	renderer.drawLineByVectors(eye3+vec3(-0.05,0,0),eye3+vec3(0.05,0,0),(unsigned int)MAGENTA);
	renderer.drawLineByVectors(eye3+vec3(0,-0.05,0),eye3+vec3(0,0.05,0),(unsigned int)MAGENTA);
	renderer.drawLineByVectors(eye3+vec3(0,0,-0.05),eye3+vec3(0,0,0.05),(unsigned int)MAGENTA);
}


const string& Camera::getName() {
	stringstream os;
	os << "(" << _eye.x << "," << _eye.y << "," << _eye.z << ")";
	name = os.str();
	return name;
}