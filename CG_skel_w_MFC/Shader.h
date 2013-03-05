#pragma once
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <string>
#include "vec.h"
#include "mat.h"
#include "Camera.h"
#include <iostream>
#include <map>

using std::map;
using std::string;

class Shader
{
	string _vertexShader;
	string _fragmentShader;

	GLuint _programHandle;
	GLuint _vPosition;
	GLuint _tCoor;
	GLuint _vNormal;
	GLuint _kAmbiant;
	GLuint _kDiffuse;
	GLuint _kSpecular;
	GLuint _shininess;

	GLuint _projection;
	GLuint _cameraView;
	GLuint _modelView;
	
	mat4 projectionMatrix;
	mat4 cameraMatrix;
	mat4 modelViewMatrix;

	void updateProjection();
	void updateCamera();
	void updateModelView();

	map<string, string> vars; /* Mapping between application var names to shader var names, this can change between shaders*/
	virtual void buildConversionTable();
public:
	Shader(string vertexShader, string fragmentShader);
	void loadProgram();

	void enableDataPointers();
	void disableDataPointers();

	void bind();
	void unbind();
	void swapBuffers();

	/* These function names should be revised and set according to their redundancy in Scene */
	mat4 getProjection() { return projectionMatrix; }
	mat4 getCamera() { return cameraMatrix; }
	mat4 getModelViewMatrix() { return modelViewMatrix; }

	void setProjection(mat4 projection) {
		projectionMatrix = projection;
		updateProjection();
	}

	void setCamera(mat4 camera) {
		cameraMatrix = camera;
		updateCamera();
	}

	void setCameraParams(Camera *camera) {
		bind();
		setProjection(camera->getProjection());
		setCamera(camera->getInverseTransformation());
		unbind();
	}

	void setModelView(mat4 modelView) {
		modelViewMatrix = modelView;
		updateModelView();
	}
	/******************************************************************************************/

	GLuint getProgram();
	void vPositionPointer(const GLvoid *data);
	void vNormalPointer(const GLvoid *data);
	void texturePointer(const GLvoid *data);

	void kAmbiantPointer(const GLvoid *data);
	void kDiffusePointer(const GLvoid *data);
	void kSpecularPointer(const GLvoid *data);

	void shininessPointer(const GLvoid *data);

	GLuint buildVAO(const map<string, GLuint>& VBOs);

	void checkError(bool except = true);
	~Shader(void);

	const string& getName() { return _vertexShader; }
};

