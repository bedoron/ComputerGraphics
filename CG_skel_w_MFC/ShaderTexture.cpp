#include "stdafx.h"
#include "ShaderTexture.h"
#include "Texture.h"
#include "Model.h"
#include <sstream>

using std::stringstream;

ShaderTexture::ShaderTexture(string name, string vertexShader, string fragmentShader,map<string, Texture*> &defaults): 
	Shader(name, vertexShader, fragmentShader, true), _defaults(defaults) {
}


ShaderTexture::~ShaderTexture(void) {
}

void ShaderTexture::postBuildConversionTable() {
	vars["vtexture"]		= "tCoor";
}

void ShaderTexture::setTextures(Model* model) {
	const map<string, Texture*> &modelTexture = model->getTextures();
	
	// Check to see if we have all needed texture units defined, if not, set defaults
	map<string, Texture*>::const_iterator defs = _defaults.begin();
	for(map<string, string>::iterator it = textures.begin(); it != textures.end(); ++it) {
		if(modelTexture.find(it->first)==modelTexture.end()) {
			model->setTexture(it->first,  (defs++)->second); // Set some arbitrary default
			//model->setTexture(it->first, _defaults.begin()->second); // Set some arbitrary default
		}
	}
}

void ShaderTexture::setTextureRealName(const string& shader_name) {
	if(vars.size()!=0)
		throw runtime_error("Shader already initialized!");
	stringstream name;
	name << "sampler" << textures.size() ;

	textures[name.str()] = shader_name;
}