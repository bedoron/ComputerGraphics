#include <exception>
#include "stdafx.h"
#include "Texture.h"
#include "pngLib\PngWrapper.h"
#include <vector>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include "Shader.h"

using std::string;
using std::cerr;
using std::runtime_error;
using std::vector;
using std::stringstream;

#define INDEX_PNG(width,x,y,c) (((y)+(x)*(width))*3+(c))
Texture::Texture(const string& file, const string& path): name(file), handler(-1), width(0), height(0), textureUnit(-1)  {
	static int TEXTURE_UNIT = 0; // Texture unit enumerator
	string texturesPath = path; 
	if(path == string())
		texturesPath = string("..\\Textures\\") + name;
	PngWrapper png((texturesPath).c_str());
	if(!png.ReadPng()) // Gigiddy gigiddy allright!
		throw runtime_error("Couldn't read PNG file");

	width = png.GetWidth();
	height = png.GetHeight();
	
	image = new GLubyte[width*height*3];
	for(int i=0; i < height; ++i) {
		for(int j=0; j < width; ++j) {
			int color = png.GetValue(j, height -i -1);
			image[INDEX_PNG(width, i, j ,0)] = GET_R(color);
			image[INDEX_PNG(width, i, j ,1)] = GET_G(color);
			image[INDEX_PNG(width, i, j ,2)] = GET_B(color);
			
		}
	}

	glGenTextures(1, &handler);
	if(handler == -1)
		throw runtime_error("Couldn't generate texture handler");
	
	glBindTexture(GL_TEXTURE_2D, handler);
	//gltexparameterf(gl_texture_2d, gl_texture_min_filter, gl_linear_mipmap_nearest);
	//gltexparameterf(gl_texture_2d, gl_texture_mag_filter, gl_linear);
	//gltexparameterf(gl_texture_2d, gl_texture_wrap_s, gl_repeat);
	//gltexparameterf(gl_texture_2d, gl_texture_wrap_t, gl_repeat);


	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	Shader::checkError(name + "::Texture image loading failed");

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // Allow texture to coexist with fragments

	glGenerateMipmap(GL_TEXTURE_2D);
	Shader::checkError(name + "::Texture mipmap gen failed");


	textureUnit = TEXTURE_UNIT++; 

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenSamplers(1, &sampler);
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

}

Texture::~Texture(void)
{
	glDeleteTextures(1, &handler); // delete buffer
	delete[] image;
}

void Texture::bind(GLuint texUnit) {
	GLuint bindTo = GL_TEXTURE0 + textureUnit;
	if(texUnit!=-1) bindTo = texUnit;
	stringstream buff;
	buff << name <<"::Texture binding to texunit " << bindTo << " failed";
	glActiveTexture( bindTo );

	glBindSampler(texUnit, sampler);
	glBindTexture(GL_TEXTURE_2D, handler);
	Shader::checkError(buff.str());
}

void Texture::unbind() {
	//glBindTexture(GL_TEXTURE_2D, 0); -- Might cause a bug
}