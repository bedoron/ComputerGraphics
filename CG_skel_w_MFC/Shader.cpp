#include "stdafx.h"
#include "Shader.h"
#include "InitShader.h"
#include <exception>
#include <cassert>
#include "Model.h"
#include <sstream>

using std::stringstream;
using std::exception;
using std::runtime_error;
using std::cerr;

Shader::Shader(string name, string vertexShader, string fragmentShader, bool textures): 
	_name(name), _vertexShader(vertexShader), _fragmentShader(fragmentShader), _programHandle(0), _has_textures(textures) {

	vboElmSize["vertices"]	= 4;
	vboElmSize["normals"]	= 4;
	vboElmSize["ambient"]	= 4;
	vboElmSize["diffuse"]	= 4;
	vboElmSize["specular"]	= 4;
	vboElmSize["shine"]		= 1;

	vboElmSize["vtexture"]	= 2;

	vboElmSize["projection"]	= 16;
	vboElmSize["camera"]		= 16;
	vboElmSize["model"]			= 16;
	vboElmSize["eye"]			= 4;
	vboElmSize["time"]			= 1;
	vboElmSize["colors"]		= 4;
	vboElmSize["positions"]		= 1;
	vboElmSize["num_lights"]	= 1;
	vboElmSize["globalAmbient"] = 4;
}

GLuint Shader::getProgram() {
	return _programHandle; 
}

bool Shader::hasTextures() {
	return _has_textures;
}


void Shader::setGlobalAmbience(vec4 globalAmbience) {
	bind();
	glUniform4fv( handlers["globalAmbient"], 1, globalAmbience);
	checkError();
	_globalAmbience = globalAmbience;
	unbind();
}


Shader& Shader::operator<<(vector<Light*> &lights) {
	GLuint num_lights = lights.size();
	bind();
	glUniform1i(handlers["num_lights"], num_lights);
	

	GLfloat *colors = new GLfloat[4*lights.size()];
	GLfloat *positions = new GLfloat[4*lights.size()];

	for(int i=0; i < lights.size(); ++i) {
		colors[4*i+0] = lights[i]->color.x;
		colors[4*i+1] = lights[i]->color.y;
		colors[4*i+2] = lights[i]->color.z;
		colors[4*i+3] = lights[i]->color.w;

		positions[4*i + 0] = lights[i]->position.x;
		positions[4*i + 1] = lights[i]->position.y;
		positions[4*i + 2] = lights[i]->position.z;
		positions[4*i + 3] = lights[i]->position.w;
	}

	glUniform4fv(handlers["colors"], num_lights, colors);
	checkError();
	glUniform4fv(handlers["positions"], num_lights, positions);
	checkError();

	delete[] colors;
	delete[] positions;

	unbind();
	return *this;
}

void Shader::buildConversionTable() {
	vars["vertices"]	= "vPosition";
	vars["normals"]		= "vNormal";
	vars["ambient"]		= "kambiant";
	vars["diffuse"]		= "kdiffuse";
	vars["specular"]	= "kspecular";
	vars["shine"]		= "shininess";
	
	uniforms["projection"]	= "Projection";
	uniforms["camera"]		= "CameraView";
	uniforms["model"]		= "ModelView";
	uniforms["eye"]			= "eye";
	uniforms["time"]		= "time";

	// Build lights uniform
	uniforms["globalAmbient"]	="globalAmbient";
	uniforms["num_lights"]		="num_lights";
	uniforms["positions"]		="positions";
	uniforms["colors"]			="colors";

	postBuildConversionTable();
}

void Shader::postBuildConversionTable() { 
	// Removal/Addition of redundant attributes should occur here
};

void Shader::checkHandler(const string& var, const GLuint handle) {
	if(handle == -1)
		throw runtime_error(_name + ": " + var + " Initialization failed, handler is -1");
	try {
		checkError();
	} catch(exception &e) {
		throw runtime_error(_name + ": " + var + " Initialization failed, OPENGL died with " + e.what());
	}
}

void Shader::reload() {
	handlers.clear();
	glDeleteProgram(_programHandle);
	loadProgram();
//	updateProjection();
//	updateCamera();
}

void Shader::loadProgram() {
	cerr << "loading " << _vertexShader << " & " << _fragmentShader << "\n";
	_programHandle = InitShader(("..\\Shaders\\" + _vertexShader).c_str(), ("..\\Shaders\\" + _fragmentShader).c_str()); /* this WILL load the shader as active */
	GLenum error = glGetError();
	if(error != GL_NO_ERROR) {
		throw exception(_name.c_str());
	}
	lightColors.clear();
	lightPositions.clear();
	handlers.clear();
	buildConversionTable();

	glUseProgram(_programHandle);
	glEnable(GL_DEPTH_TEST);

	map<string, string>::const_iterator it;
	for(it=vars.begin(); it != vars.end(); ++it) {
		GLuint handle = glGetAttribLocation(_programHandle, it->second.c_str());
		checkHandler(it->second, handle);
		handlers[it->first] = handle; 
	}

	for(it=uniforms.begin(); it != uniforms.end(); ++it) {
		GLuint handle = glGetUniformLocation(_programHandle, it->second.c_str());
		checkHandler(it->second, handle);
		handlers[it->first] = handle; 
	}

	int index = UNIFORM_BINDING_POINT;
	for(it=ublocks.begin(); it != ublocks.end(); ++it) {
		GLuint handle = glGetUniformBlockIndex(_programHandle, it->second.c_str());
		checkHandler(it->second,  handle);
		handlers[it->first] = handle;
		glUniformBlockBinding(_programHandle, handle, index);
		checkError();
		index++;
	}

	for(it=textures.begin(); it != textures.end(); ++it) {
		GLuint handle = glGetUniformLocation(_programHandle, it->second.c_str());
		checkHandler(it->second, handle);
		handlers[it->first] = handle;
	}

	glUseProgram(0); // unload
}

void Shader::swapBuffers() {
	glutSwapBuffers();
}

void Shader::checkError(string name, bool except) {
	GLenum error = glGetError();
	if(error != GL_NO_ERROR) {
		stringstream errstr; 
		errstr << "GLError: " << name << ": " <<error;
		cerr <<  errstr.str() << "\n";
		if(except)
			throw exception(errstr.str().c_str());
	}
}

void Shader::checkError(bool except) {
	Shader::checkError(_name, except);
}

void Shader::bind() {
	glUseProgram(_programHandle);
	checkError();
}

void Shader::unbind() {
	glUseProgram(0);
}

void Shader::enableDataPointers()  {
	map<string, string>::const_iterator it;
	for(it = vars.begin(); it != vars.end(); ++it) {
		glEnableVertexAttribArray(handlers[it->first]);
		checkError();
	}
}

void Shader::disableDataPointers() {
	map<string, string>::const_iterator it;
	for(it = vars.begin(); it != vars.end(); ++it) {
		glDisableVertexAttribArray(handlers[it->first]);
		checkError();
	}
}


GLuint Shader::buildVAO(const map<string, GLuint>& VBOs) {
	map<string, GLuint>::const_iterator it;

	GLuint vao;
	GLuint attrib_handler = -1;
	GLuint vbo = -1;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao); 
	checkError();
	enableDataPointers();
	checkError();

	for(it = VBOs.begin(); it != VBOs.end(); ++it) {
		if(vars.find(it->first)==vars.end())
			continue;
		attrib_handler = handlers[it->first];
		vbo = it->second;
		assert( (attrib_handler != -1) && (vbo != -1) );
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		checkError();
		GLuint elms = vboElmSize[it->first];
		glVertexAttribPointer( attrib_handler, elms, GL_FLOAT, GL_FALSE, 0, 0);
		checkError();
	}

	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);
	return vao;
}

void Shader::vPositionPointer(const GLvoid *data) {
	glVertexAttribPointer( handlers["vertices"], 4, GL_FLOAT, GL_FALSE, 0, data );
}

void Shader::vNormalPointer(const GLvoid *data) {
	glVertexAttribPointer( handlers["normals"], 4, GL_FLOAT, GL_FALSE, 0, data );
}

void Shader::texturePointer(const GLvoid *data) {
	glVertexAttribPointer( handlers["texture"], 2, GL_FLOAT, GL_FALSE, 0, data );
}

void Shader::kAmbiantPointer(const GLvoid *data){
	glVertexAttribPointer( handlers["ambiant"], 4, GL_FLOAT, GL_FALSE, 0, data );
}
void Shader::kDiffusePointer(const GLvoid *data) {
	glVertexAttribPointer( handlers["diffuse"], 4, GL_FLOAT, GL_FALSE, 0, data );
}
void Shader::kSpecularPointer(const GLvoid *data) {
	glVertexAttribPointer( handlers["specular"], 4, GL_FLOAT, GL_FALSE, 0, data );
}

void Shader::shininessPointer(const GLvoid *data) {
	glVertexAttribPointer( handlers["shine"], 1, GL_FLOAT, GL_FALSE, 0, data );
}

Shader::~Shader(void) {
}

void Shader::updateEye() {
	if(handlers.find("eye")==handlers.end()) return;
	glUniform4fv( handlers["eye"], 1, cameraEye );
	checkError();
}

void Shader::updateProjection() {
	//GLenum error = glGetError();
	//bind();
	glUniformMatrix4fv(	handlers["projection"], 1, GL_TRUE, projectionMatrix );
	checkError();
}

void Shader::updateCamera() {
	//bind();
	glUniformMatrix4fv(	handlers["camera"], 1, GL_TRUE, cameraMatrix);
	checkError();
}

void Shader::updateModelView() {
	glUniformMatrix4fv(	handlers["model"], 1, GL_TRUE, modelViewMatrix );
	checkError();
}
void Shader::updateTime(bool t) 
{
	int curT = t?(float)GetTickCount()/10:0.0;
	double final = ((double)(curT % 360))*(M_PI/180);
	glUniform1f(handlers["time"],final);
	checkError();
}

void Shader::draw(Model* model,bool animate) {
	bind();
	setModelView(model->getObjectTransform());
	updateTime(animate);
	setTextures(model); // Deal with defaults
	bindTextures(model->getTextures());
	glBindVertexArray(model->getVAO());
	checkError();
	glDrawArrays(GL_TRIANGLES,0,model->getNumOfVertices());
	glBindVertexArray(0);
}

void Shader::setTextures(Model *model) { 
		/* fix this - issue: when changing shader in the model it automatically redraws the scene, this is before  it has any texture defined */
		/* possible solution - assign default textures ? */
}

void Shader::bindTextures(const map<string, Texture*>& modelTexture) {
	map<string, Texture*>::const_iterator textureIterator;
	// iterate modelTexture (it should be updated now) and bind their uniform variables
	for(textureIterator = modelTexture.begin(); textureIterator != modelTexture.end(); ++textureIterator) {
		if(handlers.find(textureIterator->first)==handlers.end()) continue; // Skip samplers which are not supported
		GLuint samplerHandle = handlers[textureIterator->first];
		GLuint texUnit = textureIterator->second->getTextureUnit();
		glUniform1i(samplerHandle, texUnit);
		textureIterator->second->bind();
		checkError();
	}

}

const map<string, string>& Shader::getSamplerNames() const { 
	return textures; 
};

const string Shader::translateSamplerName(const string& shader_name) const {
	map<string, string>::const_iterator it = textures.begin();
	for(;it != textures.end(); ++it) {
		if(it->second == shader_name)
			return it->first;
	}
	return string("");
}

/*************************** OLD FUNCTIONALITY AREA *****************************/
mat4 Shader::getProjection() { return projectionMatrix; }
mat4 Shader::getCamera() { return cameraMatrix; }
mat4 Shader::getModelViewMatrix() { return modelViewMatrix; }

void Shader::setModelView(mat4 modelView)	{	modelViewMatrix = modelView; updateModelView();		}
void Shader::setProjection(mat4 projection) {	projectionMatrix = projection;	updateProjection();	}
void Shader::setCamera(mat4 camera)			{	cameraMatrix = camera;	updateCamera();				}
void Shader::setEye(vec4 eye)				{	cameraEye = eye; updateEye();						}

void Shader::setCameraParams(Camera *camera) {
	bind();
	setProjection(camera->getProjection());
	setCamera(camera->getInverseTransformation());
	setEye(camera->getEye()); // Check why its causing errors
	unbind();
	checkError();
}