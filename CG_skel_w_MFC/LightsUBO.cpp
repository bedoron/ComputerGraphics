#include "stdafx.h"
#include "LightsUBO.h"
#include <exception>

using std::exception;

LightsUBO::LightsUBO(void)
{
	glGenBuffers(1, &_bufferHandle);
	glBindBuffer(GL_UNIFORM_BUFFER, _bufferHandle);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(LightSourcesBlock) , NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	if(glGetError()!=GLEW_OK) {
		throw exception();
	}
}

void LightsUBO::setGlobalAmbient(vec4 ambient) {
	globalAmbient = ambient;
}

LightsUBO::~LightsUBO(void)
{
	glDeleteBuffers(1, &_bufferHandle);
}

LightsUBO& LightsUBO::operator<<(vector<Light*> lights) {
	LightSourcesBlock block;
	block.globalAmbient = globalAmbient;
	block.num_lights = lights.size();
	memset(&block.lightSources, 0, 12*sizeof(LightSource));
	for(int i=0; i < block.num_lights; ++i) {
		block.lightSources[i] = (LightSource)(*(lights[i]));
	}
	int ssize = sizeof(LightSourcesBlock);
	bind();
	glBufferData(GL_UNIFORM_BUFFER,sizeof(LightSourcesBlock),(GLvoid*) &block, GL_STREAM_DRAW);
	unbind();
	return (*this);
}

void LightsUBO::bind() {
	glBindBuffer(GL_UNIFORM_BUFFER, _bufferHandle);
}

void LightsUBO::unbind() {
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void LightsUBO::bindToPoint(GLuint point) {
	glBindBufferRange(GL_UNIFORM_BUFFER, point, _bufferHandle, 0, sizeof(LightSourcesBlock));
}