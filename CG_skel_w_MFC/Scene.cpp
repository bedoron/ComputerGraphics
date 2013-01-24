#include "stdafx.h"
#include "Scene.h"
#include "MeshModel.h"
#include <string>
#include "Utils.h"

//#include "ModelData.h"
#include "InitShader.h"
using namespace std;

void Scene::loadOBJModel(MeshModel* model)
{
	_activeModel = model;
	models.push_back(model);
	draw();	
	redraw();
}

void Scene::initDefaultCamera() {
	
}
void Scene::initDefaultLight()
{
		
}
Scene::Scene():models()
{
}
void Scene::setFrustum(float left,float right,float top,float bottom,float zNear,float zFar)
{
	_zfar=zFar;
	_znear = zNear;
	_right = right;
	_left = left;
	_top = top;
	_bottom = bottom;
	mat4 p;
	p[0][0] = 2*zNear/(right-left)	; p[0][1] = 0						; p[0][2] =  (right + left)/(right - left);	; p[0][3] = 0;
	p[1][0] = 0						; p[1][1] = 2*zNear/(top-bottom)	; p[1][2] = (top + bottom)/(top - bottom)	; p[1][3] = 0;
	p[2][0] = 0						; p[2][1] = 0						; p[2][2] = -(zFar + zNear)/(zFar - zNear)	; p[2][3] = -2*zNear*zFar/(zFar-zNear);
	p[3][0] = 0						; p[3][1] = 0						; p[3][2] = -1								; p[3][3] = 0;
	_current_Projection = p;
	_currentPtype = frustum;
}
void Scene::setOrtho(float left,float right,float top,float bottom,float zNear,float zFar)
{
	_zfar=zFar;
	_znear = zNear;
	_right = right;
	_left = left;
	_top = top;
	_bottom = bottom;
  	mat4 p;
	p[0][0] = 2/(right-left); p[0][1] = 0				; p[0][2] = 0				; p[0][3] = -(right + left)/(right - left);
	p[1][0] = 0				; p[1][1] = 2/(top-bottom)	; p[1][2] = 0				; p[1][3] = -(top + bottom)/(top- bottom);
	p[2][0] = 0				; p[2][1] = 0				; p[2][2] = 2/(zNear - zFar); p[2][3] = -(zNear + zFar)/(zFar - zNear);
	p[3][0] = 0				; p[3][1] = 0				; p[3][2] = 0				; p[3][3] = 1;
	_current_Projection = p;
	_currentPtype = ortho;
}
void Scene::setPrespective(float fovy,float aspect,float zNear, float zFar)
{
	_fovy = fovy;
	_aspect = aspect;
	_zfar=zFar;
	_znear = zNear;
	GLfloat top = zNear*tan(M_PI* fovy/180);
	GLfloat right = top*aspect;
	mat4 c;
	c[0][0] = zNear/right;		c[0][1] = 0;				c[0][2] = 0;						c[0][3] = 0;
	c[1][0] = 0;				c[1][1] = zNear/top;		c[1][2] = 0;						c[1][3] = 0; 
	c[2][0] = 0;				c[2][1] = 0;				c[2][2] = (-zFar+zNear)/(zFar-zNear);c[2][3] = (-2*zFar*zNear)/(zFar-zNear); 
	c[3][0] = 0;				c[3][1] = 0;				c[3][2] = -1;						c[3][3] = 0; 
	_currentPtype = prespctive;
	_current_Projection = c;
}


void Scene::setZoom(GLfloat zoom)
{
	switch (_currentPtype)
	{
	case frustum:
		{
			_left -= zoom;
			_right += zoom;
			_top += zoom;
			_bottom -= zoom;
			setFrustum(_left,_right,_top,_bottom, _znear, _zfar);
			break;
		}
	case ortho:
		{
			_left -= zoom;
			_right += zoom;
			_top += zoom;
			_bottom -= zoom;
			setOrtho(_left,_right,_top,_bottom, _znear, _zfar);
			break;
		}
	case prespctive:
		{
			_fovy -= 3*zoom;
			setPrespective(_fovy,_aspect,_znear,_zfar);
			break;
		}
	}
	GLuint projection = glGetUniformLocation( program, "Projection");
	glUniformMatrix4fv(projection, 1, GL_TRUE, _current_Projection);
	
}
void Scene::draw()
{
	GLuint vao;
	glGenVertexArrays(1 ,&vao);
	glBindVertexArray( vao );

	program = InitShader( "PhongVshader.glsl","PhongFshader.glsl");
	glUseProgram( program );
	
	GLuint Camera_view = glGetUniformLocation(program,"CameraView");
	GLuint projection = glGetUniformLocation( program, "Projection");
	GLuint eyeLocation = glGetUniformLocation( program, "eye");
	glEnable(GL_DEPTH_TEST);
	
	glUniformMatrix4fv(projection, 1, GL_TRUE, _current_Projection);
	glUniformMatrix4fv(Camera_view, 1, GL_TRUE, _currentCamera);
	glUniform4f(eyeLocation,eye.x,eye.y,eye.z,eye.w);
	for(std::vector<Model*>::iterator it = models.begin(); it != models.end(); ++it)  
	{
		(*it)->drawNormal(program);
	}
	glClearColor(0.5,0.5,0.5,1);
}
void Scene::redraw()
{
	
	for(std::vector<Model*>::iterator it = models.begin(); it != models.end(); ++it)  
	{
		(*it)->reDraw(program);
	}
}
void Scene::transformActiveModel(mat4 translation)
{
	_activeModel->setObjectTransform(_activeModel->getObjectTransform() * translation);
}