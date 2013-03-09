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
	Texture(const string& file);
	~Texture(void);

	void bind();
	void unbind();

	GLuint getTextureUnit() { return textureUnit; }
	const string& getName() { return name; }

	void checkError(bool except = true);
};

