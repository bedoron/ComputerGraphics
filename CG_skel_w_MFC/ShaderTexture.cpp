#include "stdafx.h"
#include "ShaderTexture.h"
#include "Texture.h"
#include "Model.h"

ShaderTexture::ShaderTexture(string name, string vertexShader, string fragmentShader,map<string, Texture*> &defaults): 
	Shader(name, vertexShader, fragmentShader, true), _defaults(defaults) {
}


ShaderTexture::~ShaderTexture(void) {
}

void ShaderTexture::postBuildConversionTable() {
	vars["texture"]		= "tCoor";
	
	textures["sampler0"]	= "shader_sampler_0";
	textures["sampler1"]	= "shader_sampler_1";
}

void ShaderTexture::setTextures(Model* model) {
	const map<string, Texture*> &modelTexture = model->getTextures();
	
	// Check to see if we have all needed texture units defined, if not, set defaults
	for(map<string, string>::iterator it = textures.begin(); it != textures.end(); ++it) {
		if(modelTexture.find(it->first)==modelTexture.end())
			model->setTexture(it->first, _defaults.begin()->second); // Set some arbitrary default
	}
}