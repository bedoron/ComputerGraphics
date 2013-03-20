#pragma once
#include "Shader.h"
#include "Texture.h"
#include <map>

using std::map;

class ShaderTexture :
	public Shader
{
	map<string, Texture*> &_defaults; // Texture name to object
protected:
	//void buildConversionTable(); /* Populate the above */
	void postBuildConversionTable(); /* Post population hook */
	void setTextures(Model* model); // Activate Texture units from model, use defaults if they don't exist
public:
	ShaderTexture(string name, string vertexShader, string fragmentShader, map<string, Texture*> &defaults);
	~ShaderTexture(void);

	void setTextureRealName(const string& shader_name);
};

