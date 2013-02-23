#include "stdafx.h"
#include "Shader.h"
#include "InitShader.h"
#include "pngLib/PngWrapper.h"
#include <exception>
#include <cassert>
#define INDEX_PNG(width,x,y,c) (((y)+(x)*(width))*3+(c))
using std::exception;
using std::cerr;

Shader::Shader(string vertexShader, string fragmentShader): _vertexShader(vertexShader), _fragmentShader(fragmentShader), _programHandle(0), _vPosition(0), _tCoor(0), _vNormal(0), _kAmbiant(0), 
	_kDiffuse(0), _kSpecular(0), _shininess(0), _projection(0), _cameraView(0), _modelView(0) ,textures(){}

GLuint Shader::getProgram() {
	return _programHandle; 
}

void Shader::loadProgram() {
	_programHandle = InitShader(_vertexShader.c_str(), _fragmentShader.c_str()); /* this WILL load the shader as active */
	GLenum error = glGetError();
	if(error != GL_NO_ERROR) {
		throw exception();
	}
	
	glUseProgram(_programHandle);
	glEnable(GL_DEPTH_TEST);

	

	_vPosition = glGetAttribLocation(_programHandle, "vPosition");
	
	_vNormal = glGetAttribLocation(_programHandle, "vNormal");
	checkError();
	assert(_vNormal != -1);
	_tCoor = glGetAttribLocation(_programHandle, "tCoor");
	checkError();
	assert(_tCoor != -1);
	_kAmbiant = glGetAttribLocation(_programHandle,"kambiant");
	checkError();
	assert(_kAmbiant != -1);
	_kDiffuse = glGetAttribLocation(_programHandle, "kdiffuse");
	checkError();
	assert(_kDiffuse != -1);
	_kSpecular = glGetAttribLocation(_programHandle, "kspecular");
	checkError();
	assert(_kSpecular != -1);
	_shininess = glGetAttribLocation(_programHandle, "shininess");
	checkError();
	assert(_vNormal != -1);
	_projection =  glGetUniformLocation(_programHandle, "Projection");
	assert(_projection != -1);
	checkError();
	_cameraView = glGetUniformLocation(_programHandle, "CameraView");
	assert(_cameraView != -1);
	checkError();
	_modelView = glGetUniformLocation(_programHandle, "ModelView");
	assert(_modelView != -1);
	checkError();
}

void Shader::swapBuffers() {
	glutSwapBuffers();
}

void Shader::checkError(bool except) {
	GLenum error = glGetError();
	if(error != GL_NO_ERROR) {
		cerr << "GLError " << error << "\n";
		if(except)
			throw exception();
	}
}

void Shader::bind() {
	glUseProgram(_programHandle);
	checkError();
}

void Shader::unbind() {
	glUseProgram(0);
}

void Shader::enableDataPointers()  {
	glEnableVertexAttribArray(_vPosition);
	glEnableVertexAttribArray(_vNormal);
	glEnableVertexAttribArray(_tCoor);
	glEnableVertexAttribArray(_kAmbiant);
	glEnableVertexAttribArray(_kDiffuse);
	glEnableVertexAttribArray(_shininess);
}

void Shader::disableDataPointers() {
	glDisableVertexAttribArray(_vPosition);
	glDisableVertexAttribArray(_vNormal);
	glDisableVertexAttribArray(_tCoor);
	glDisableVertexAttribArray(_kAmbiant);
	glDisableVertexAttribArray(_kDiffuse);
	glDisableVertexAttribArray(_shininess);
}

void Shader::vPositionPointer(const GLvoid *data) {
	glVertexAttribPointer( _vPosition, 4, GL_FLOAT, GL_FALSE, 0, data );
}

void Shader::vNormalPointer(const GLvoid *data) {
	glVertexAttribPointer( _vNormal, 4, GL_FLOAT, GL_FALSE, 0, data );
}

void Shader::texturePointer(const GLvoid *data) {
	glVertexAttribPointer( _tCoor, 2, GL_FLOAT, GL_FALSE, 0, data );
}

void Shader::kAmbiantPointer(const GLvoid *data){
	glVertexAttribPointer( _kAmbiant, 4, GL_FLOAT, GL_FALSE, 0, data );
}
void Shader::kDiffusePointer(const GLvoid *data) {
	glVertexAttribPointer( _kDiffuse, 4, GL_FLOAT, GL_FALSE, 0, data );
}
void Shader::kSpecularPointer(const GLvoid *data) {
	glVertexAttribPointer( _kSpecular, 4, GL_FLOAT, GL_FALSE, 0, data );
}

void Shader::shininessPointer(const GLvoid *data) {
	glVertexAttribPointer( _shininess, 1, GL_FLOAT, GL_FALSE, 0, data );
}

Shader::~Shader(void) {
}


void Shader::updateProjection() {
	glUniformMatrix4fv(	_projection, 1, GL_TRUE, projectionMatrix );
	checkError();
}

void Shader::updateCamera() {
	glUniformMatrix4fv(	_cameraView, 1, GL_TRUE, cameraMatrix);
	checkError();
}

void Shader::updateModelView() {
	glUniformMatrix4fv(	_modelView, 1, GL_TRUE, modelViewMatrix );
	checkError();
}
void Shader::loadPng (const char* fileName)
{
	GLuint texture;
	PngWrapper pngFile(fileName);
	pngFile.ReadPng();
	int width = pngFile.GetWidth();
	int height = pngFile.GetHeight();
	GLubyte* texels = new GLubyte[width*height*3];
	for (int i=0; i< height; ++i)
	{
		for (int j=0; j< width; j++)
		{
			int value = pngFile.GetValue(j, height -i -1);
			texels[INDEX_PNG(width, i, j ,0)] = GET_R(value);
			texels[INDEX_PNG(width, i, j ,1)] = GET_G(value);
			texels[INDEX_PNG(width, i, j ,2)] = GET_B(value);
		}
	}
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,texels);

	glGenerateMipmap(GL_TEXTURE_2D);
	textures.push_back(texture);
}