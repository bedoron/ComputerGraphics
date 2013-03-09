#include <exception>
#include "stdafx.h"
#include "Texture.h"
#include "pngLib\PngWrapper.h"
#include <vector>
#include <exception>
#include <iostream>
#include <string>

using std::string;
using std::cerr;
using std::runtime_error;
using std::vector;

Texture::Texture(const string& file): name(file), handler(-1), width(0), height(0), textureUnit(-1)  {
	static int TEXTURE_UNIT = 0; // Texture unit enumerator

	PngWrapper png(name.c_str());
	if(!png.ReadPng()) // Gigiddy gigiddy allright!
		throw runtime_error("Couldn't read PNG file");

	width = png.GetWidth();
	height = png.GetHeight();
	
	GLubyte *image = new GLubyte[width*height*3];
	for(int i=0; i < width; ++i) {
		for(int j=0; j < height; ++j) {
			int color = png.GetValue(i, j);
			image[i*width + 3*j + 0] = GET_R(color);
			image[i*width + 3*j + 1] = GET_G(color);
			image[i*width + 3*j + 2] = GET_B(color);
		}
	}

	glGenTextures(1, &handler);
	if(handler == -1)
		throw runtime_error("Couldn't generate texture handler");
	
	glBindTexture(GL_TEXTURE_2D, handler);
	checkError();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	checkError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	checkError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	checkError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	checkError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	checkError();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // Allow texture to coexist with fragments
	checkError();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	checkError();
	glGenerateMipmap(GL_TEXTURE_2D);
	checkError();
	glBindTexture(GL_TEXTURE_2D, 0);
	// p
	textureUnit = TEXTURE_UNIT++; 
	// v
	glActiveTexture( GL_TEXTURE0 + textureUnit);
	checkError();
	glBindTexture(GL_TEXTURE_2D, handler);
	checkError();
	delete[] image;
}

void Texture::checkError(bool except) {
	GLenum error = glGetError();
	if(error != GL_NO_ERROR) {
		cerr << "GLError " << error << "\n";
		if(except)
			throw runtime_error("Error GL");
	}
}


Texture::~Texture(void)
{
	glDeleteTextures(1, &handler);
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, handler);
}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}