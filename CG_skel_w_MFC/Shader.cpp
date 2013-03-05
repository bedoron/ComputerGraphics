#include "stdafx.h"
#include "Shader.h"
#include "InitShader.h"
#include <exception>
#include <cassert>

using std::exception;
using std::cerr;

Shader::Shader(string vertexShader, string fragmentShader): _vertexShader(vertexShader), _fragmentShader(fragmentShader), _programHandle(0), _vPosition(0), _tCoor(0), _vNormal(0), _kAmbiant(0), 
	_kDiffuse(0), _kSpecular(0), _shininess(0), _projection(0), _cameraView(0), _modelView(0) {}

GLuint Shader::getProgram() {
	return _programHandle; 
}

void Shader::buildConversionTable() {
	vars["vertices"]	= "vPosition";
	vars["normals"]		= "vNormal";
	vars["ambient"]		= "kambiant";
	vars["diffuse"]		= "kdiffuse";
	vars["specular"]	= "kspecular";
	vars["shine"]		= "shininess";
	vars["projection"]	= "Projection";
	vars["camera"]		= "CameraView";
	vars["model"]		= "ModelView";
}

void Shader::loadProgram() {
	_programHandle = InitShader(_vertexShader.c_str(), _fragmentShader.c_str()); /* this WILL load the shader as active */
	GLenum error = glGetError();
	if(error != GL_NO_ERROR) {
		throw exception();
	}
	buildConversionTable();

	glUseProgram(_programHandle);
	glEnable(GL_DEPTH_TEST);

	_vPosition = glGetAttribLocation(_programHandle, vars["vertices"].c_str());
	
	_vNormal = glGetAttribLocation(_programHandle, vars["normals"].c_str());
	checkError();
	assert(_vNormal != -1);
	//_tCoor = glGetAttribLocation(_tCoor, "tCoor");
	_kAmbiant = glGetAttribLocation(_programHandle, vars["ambient"].c_str());
	checkError();
	assert(_kAmbiant != -1);
	_kDiffuse = glGetAttribLocation(_programHandle, vars["diffuse"].c_str());
	checkError();
	assert(_kDiffuse != -1);
	_kSpecular = glGetAttribLocation(_programHandle, vars["specular"].c_str());
	checkError();
	assert(_kSpecular != -1);
	_shininess = glGetAttribLocation(_programHandle, vars["shine"].c_str());
	checkError();
	assert(_vNormal != -1);
	_projection =  glGetUniformLocation(_programHandle, vars["projection"].c_str());
	assert(_projection != -1);
	checkError();
	_cameraView = glGetUniformLocation(_programHandle, vars["camera"].c_str());
	assert(_cameraView != -1);
	checkError();
	_modelView = glGetUniformLocation(_programHandle, vars["model"].c_str());
	assert(_modelView != -1);
	checkError();

	glUseProgram(0); // unload
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
	//glEnableVertexAttribArray(_tCoor);
	glEnableVertexAttribArray(_kAmbiant);
	glEnableVertexAttribArray(_kDiffuse);
	glEnableVertexAttribArray(_shininess);
}

void Shader::disableDataPointers() {
	glDisableVertexAttribArray(_vPosition);
	glDisableVertexAttribArray(_vNormal);
	/*glDisableVertexAttribArray(_tCoor);*/
	glDisableVertexAttribArray(_kAmbiant);
	glDisableVertexAttribArray(_kDiffuse);
	glDisableVertexAttribArray(_shininess);
}


GLuint Shader::buildVAO(const map<string, GLuint>& VBOs) {
	map<GLuint,GLuint> vars_to_vbos;
	map<GLuint,GLuint>::const_iterator vtvit;
	bind();
	
	// Find variables for VBOs and map them
	for(map<string, GLuint>::const_iterator it=VBOs.begin(); it != VBOs.end(); ++it) {
		GLuint handle;
		const char* name ; // Convert app name to shader name
		try {
			const string& appName = it->first;
			name = vars.at(appName).c_str(); 
		} catch(out_of_range e) { // skip variables that this shader doesn't support
			continue;
		}
		switch(it->first[0]) {
		case 'u':	handle = glGetUniformLocation(_programHandle, name);	break;	/* uniform variables */
		case 't':   throw exception();										break; /* texture sampler - not implemented yet */
		default:	handle = glGetAttribLocation(_programHandle, name);		break;	/* all others */
		}
		checkError();
		vars_to_vbos[handle] = it->second;
	}

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao); 
	checkError();
	enableDataPointers();
	checkError();

	for(vtvit=vars_to_vbos.begin(); vtvit != vars_to_vbos.end(); ++vtvit) {
		glBindBuffer(GL_ARRAY_BUFFER, vtvit->second);
		checkError();
		glVertexAttribPointer( vtvit->first, 4, GL_FLOAT, GL_FALSE, 0, 0);
		checkError();
	}

	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);
	return vao;
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
	//GLenum error = glGetError();
	//bind();
	glUniformMatrix4fv(	_projection, 1, GL_TRUE, projectionMatrix );
	checkError();
}

void Shader::updateCamera() {
	//bind();
	glUniformMatrix4fv(	_cameraView, 1, GL_TRUE, cameraMatrix);
	checkError();
}

void Shader::updateModelView() {
	glUniformMatrix4fv(	_modelView, 1, GL_TRUE, modelViewMatrix );
	checkError();
}