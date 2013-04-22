#pragma once
#include "GL/glew.h"
#include <string>
#include "pngLib\PngWrapper.h"

using std::string;

class Texture
{
	string name;	// will be filename
	GLuint handler;
	int width;
	int height;
	GLuint textureUnit;
	GLubyte *image;
public:
//	Texture(const string& file);
	Texture(const string& file, const string& path = string());
	~Texture(void);

	void bind(GLuint texUnit = -1); // -1 means to use self asigned TEXUNIT, might crash your puny GPU
	void unbind();

	GLuint getTextureUnit() { return textureUnit; }
	const string& getName() const { return name; }


};

