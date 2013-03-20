#pragma once
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <string>
#include "vec.h"
#include "mat.h"
#include "Camera.h"
#include <iostream>
#include <map>
#include "Texture.h"
#include "LightsUBO.h";

class Model; // forward decleration

using std::map;
using std::string;

class Shader
{
	string _name;
	string _vertexShader;
	string _fragmentShader;

	GLuint _programHandle;

	mat4 projectionMatrix;
	mat4 cameraMatrix;
	mat4 modelViewMatrix;
	vec4 cameraEye;

	bool _has_textures;

	void updateProjection();
	void updateCamera();
	void updateModelView();
	void updateTime(bool);
	void updateEye();
	void bindTextures(const map<string, Texture*>& modelTexture);

	map<string, GLuint> handlers; // Handlers for all variables
	map<string, int>	vboElmSize; /* Mapping between application var to their Pointer attribute size, this shouldn't change */

	void checkHandler(const string& var, const GLuint handle);
protected:
	map<string, string> vars;		/* Mapping between application var names to shader var names, this can change between shaders*/
	map<string, string> textures;	/* Mapping between application texture names to shader texture names */
	map<string, string> uniforms;	/* Mapping between application uniform names to shader uniform names */
	map<string, string> ublocks;	/* Mapping between application uniform blocks names to shader uniform names */

	virtual void buildConversionTable(); /* Populate the above */
	virtual void postBuildConversionTable(); /* Post population hook */
	virtual void setTextures(Model* model);  /* Activate Texture units from model, use defaults if they don't exist */

public:
	static const GLuint UNIFORM_BINDING_POINT = 5;

	Shader(string name, string vertexShader, string fragmentShader, bool textures = false);
	void loadProgram();

	void enableDataPointers();
	void disableDataPointers();

	void bind();
	void unbind();
	void swapBuffers();

	bool hasTextures();

	/*************************** OLD FUNCTIONALITY AREA *****************************/
	mat4 getProjection();
	mat4 getCamera();
	mat4 getModelViewMatrix();

	void setModelView(mat4 modelView);
	void setProjection(mat4 projection);
	void setCamera(mat4 camera);
	void setEye(vec4 eye);
	void setCameraParams(Camera *camera);
	/*********************************************************************************/

	GLuint getProgram();
	void vPositionPointer(const GLvoid *data);
	void vNormalPointer(const GLvoid *data);
	void texturePointer(const GLvoid *data); // To be removed

	void kAmbiantPointer(const GLvoid *data);
	void kDiffusePointer(const GLvoid *data);
	void kSpecularPointer(const GLvoid *data);
	void shininessPointer(const GLvoid *data);
	void timePointer(const GLvoid *data);

	GLuint buildVAO(const map<string, GLuint>& VBOs);
	

	void checkError(bool except = true);
	~Shader(void);

	void draw(Model* model,bool animate);

	const string& getName() { return _name; }
	const map<string, string>& getSamplerNames() const;
	const string translateSamplerName(const string& shader_name) const; // translate from shader file to app name

	void updateHandler(GLuint shader_handle, GLuint buffer);
};

