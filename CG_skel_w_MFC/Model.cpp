#include "StdAfx.h"
#include "Model.h"
#include "vec.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Utils.h"

using namespace std;

struct FaceIdcs
{
	int v[4];
	int vn[4];
	int vt[4];

	FaceIdcs()
	{
		for (int i=0; i<4; i++)
			v[i] = vn[i] = vt[i] = 0;
	}

	FaceIdcs(std::istream & aStream)
	{
		for (int i=0; i<4; i++)
			v[i] = vn[i] = vt[i] = 0;

		char c;
		for(int i = 0; i < 3; i++)
		{
			aStream >> std::ws >> v[i] >> std::ws;
			if (aStream.peek() != '/')
				continue;
			aStream >> c >> std::ws;
			if (aStream.peek() == '/')
			{
				aStream >> c >> std::ws >> vn[i];
				continue;
			}
			else
				aStream >> vt[i];
			if (aStream.peek() != '/')
				continue;
			aStream >> c >> vn[i];
		}
	}
};

vec3 vec3fFromStream(std::istream & aStream)
{
	float x, y, z;
	aStream >> x >> std::ws >> y >> std::ws >> z;
	return vec3(x, y, z);
}

vec2 vec2fFromStream(std::istream & aStream)
{
	float x, y;
	aStream >> x >> std::ws >> y;
	return vec2(x, y);
}

Model::Model(OBJItem modelItem):objItem(modelItem),_world_transform(mat4()),_color(vec3(255,255,255)),
	_kAmbiant(vec3(1,1,1)),_kDiffuze(vec3(1,1,1)),_kspecular(vec3(1,1,1)),shine(18),_numOfColors(255),_cartoonize(false)
{
	copyData();
	generateBuffers();
}

Model::~Model(void)
{
}

GLuint Model::getVAO() {
	return _vao;
}

void Model::draw(Shader *shader) {
	if(_shader == 0) throw exception();
	_shader->draw(this);
}

void Model::setObjectTransform(mat4 worldTransform)
{
	_world_transform = worldTransform;
}
mat4 Model::getObjectTransform()
{
	return _world_transform;
}
vec3 Model::getModelCenter()
{
	return vec3(_world_transform[0][3],_world_transform[1][3],_world_transform[2][3]);
}

void Model::scale(const vec3& scaler) {
	_world_transform = _world_transform * Scale(scaler);
}

void Model::rotate(const vec3& rotors) {
	_world_transform = _world_transform * RotateX(rotors.x) * RotateY(rotors.y) * RotateZ(rotors.z);
}

void Model::setShader(Shader* shader) {
	_shader = shader;
	pair<Shader*, GLuint> *shaderVaoPair;
	try{
		shaderVaoPair = &vaos.at(shader->getName());
		_vao = shaderVaoPair->second; // Shader already has a VAO
	} catch(out_of_range e) { // build this vao and set it as active
		buildVAO();
		vaos[shader->getName()] = pair<Shader*, GLuint>(shader, _vao);
	} 
}

void Model::setTexture(const string &sampler, Texture* texture) {
	textures[sampler] = texture;
}

const map<string, Texture*>& Model::getTextures() {
	return textures;
}


void Model::generateBuffers() {
	
	GLuint buffers[7];
	glGenBuffers(7, buffers);
	VBOs["vertices"] = buffers[0];
	VBOs["normals"] = buffers[1];
	VBOs["ambient"] = buffers[2];
	VBOs["diffuse"] = buffers[3];
	VBOs["specular"] = buffers[4];
	VBOs["shine"] = buffers[5];
	VBOs["vtexture"] = buffers[6];

	int points = objItem.faces.size()*3;
	glBindBuffer(GL_ARRAY_BUFFER, VBOs["vertices"]);
	glBufferData( GL_ARRAY_BUFFER, sizeof(vec4)*points, verticesArray4, GL_STATIC_DRAW );

	glBindBuffer(GL_ARRAY_BUFFER, VBOs["normals"]);
	glBufferData( GL_ARRAY_BUFFER, sizeof(vec4)*points, normalsArray4, GL_STATIC_DRAW );

	glBindBuffer(GL_ARRAY_BUFFER, VBOs["ambient"]);
	glBufferData( GL_ARRAY_BUFFER, sizeof(vec4)*points, _kAmbiantArray, GL_STATIC_DRAW );

	glBindBuffer(GL_ARRAY_BUFFER, VBOs["diffuse"]);
	glBufferData( GL_ARRAY_BUFFER, sizeof(vec4)*points, _kDifuseArray, GL_STATIC_DRAW );

	glBindBuffer(GL_ARRAY_BUFFER, VBOs["specular"]);
	glBufferData( GL_ARRAY_BUFFER, sizeof(vec4)*points, _kSpecularArray, GL_STATIC_DRAW );

	glBindBuffer(GL_ARRAY_BUFFER, VBOs["shine"]);
	glBufferData( GL_ARRAY_BUFFER, sizeof(GLfloat)*points, _shineArray, GL_STATIC_DRAW );
	
	glBindBuffer(GL_ARRAY_BUFFER, VBOs["vtexture"]);
	glBufferData( GL_ARRAY_BUFFER, sizeof(vec2)*points, _vtArray, GL_STATIC_DRAW );
	
}


void Model::buildVAO() { // happens per Shader. VAO per (Model, Shader) pair
	if(_shader == NULL)
		throw exception();
	_vao = _shader->buildVAO(VBOs);
}

void Model::copyData()
{
	verticesArray4 = new vec4[objItem.faces.size()*3];
	normalsArray4 = new vec4[objItem.faces.size()*3];
	_kAmbiantArray = new vec4[objItem.faces.size()*3];
	_kDifuseArray = new vec4[objItem.faces.size()*3];
	_kSpecularArray = new vec4[objItem.faces.size()*3];
	_shineArray = new GLfloat[objItem.faces.size()*3];
	_vtArray = new vec2[objItem.faces.size()*3];
	int k=0;
	for(std::vector<Face>::iterator it = objItem.faces.begin() ;it != objItem.faces.end(); ++it) 
	{
		Face tmp = (*it);
		vec3 faceVertices = tmp.getVertices();
		_vtArray[k] = tmp.getVtX();
		_kAmbiantArray[k] = tmp.getKAmbiant();
		_kDifuseArray[k] = tmp.getKDiffuze();
		_kSpecularArray[k] = tmp.getKSpecular();
		
		vec3 tmpNormal=tmp.getVnX();
		tmpNormal = (tmpNormal==vec3(0,0,0))?objItem.getCalculatedNormal(faceVertices.x):tmpNormal;
		normalsArray4[k] = vec4(tmpNormal);

		_shineArray[k] = tmp.getNS();
		verticesArray4[k++]=vec4(tmp.getVecX());
		
		_vtArray[k] = tmp.getVtY();
		_kAmbiantArray[k] = tmp.getKAmbiant();
		_kDifuseArray[k] = tmp.getKDiffuze();
		_kSpecularArray[k] = tmp.getKSpecular();
		tmpNormal=tmp.getVnY();
		tmpNormal = (tmpNormal==vec3(0,0,0))?objItem.getCalculatedNormal(faceVertices.y):tmpNormal;
		normalsArray4[k] = vec4(tmpNormal);
		_shineArray[k] = tmp.getNS();
		verticesArray4[k++]=vec4(tmp.getVecY());
		
		_vtArray[k] = tmp.getVtZ();
		_kAmbiantArray[k] = tmp.getKAmbiant();
		_kDifuseArray[k] = tmp.getKDiffuze();
		_kSpecularArray[k] = tmp.getKSpecular();

		tmpNormal=tmp.getVnZ();
		tmpNormal = (tmpNormal==vec3(0,0,0))?objItem.getCalculatedNormal(faceVertices.z):tmpNormal;
		normalsArray4[k]=vec4(tmpNormal);
		_shineArray[k] = tmp.getNS();
		verticesArray4[k++]=vec4(tmp.getVecZ());
	}
}

void Model::loadBuffers() { // Load buffers into GPU

}

void Model::bindBuffers() { // Make buffers active

}