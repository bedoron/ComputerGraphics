#include "StdAfx.h"
#include "MeshModel.h"
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

MeshModel::MeshModel(OBJItem modelItem):objItem(modelItem),_world_transform(mat4()),_color(vec3(255,255,255)),
	_kAmbiant(vec3(1,1,1)),_kDiffuze(vec3(1,1,1)),_kspecular(vec3(1,1,1)),shine(18),_numOfColors(255),_cartoonize(false)
{
	copyData();
	generateBuffers();
}

MeshModel::~MeshModel(void)
{
}

void MeshModel::draw(Shader *shader) {
	if(_shader != shader) {
		/* Houston, do we have a problem ? */
		printf("Different shader, expect horrors!\n");
	}
	_shader->bind();
	_shader->setModelView(_world_transform);

	glBindVertexArray(_vao);
	_shader->checkError();
	glDrawArrays(GL_TRIANGLES,0,objItem.faces.size()*3);
	_shader->checkError();
	glBindVertexArray(0);
}

//void MeshModel::draw(Renderer& renderer)
//{
//	/*
//	glBindVertexArray(vaoID);
//	assert((objItem.vertices.size()%3)==0); // Sanity test
//	glDrawArrays(GL_TRIANGLES,0, objItem.vertices.size()/3);
//	glBindVertexArray(0);
//	*/
//	//throw exception(); // TODO: add buffer stuff here
//	/*
//	renderer.SetObjectMatrices(_world_transform);
//	objItem.setColor(_color);
//	renderer.setKAbmbiant(_kAmbiant);
//	renderer.setKDiffuze(_kDiffuze);
//	renderer.setKspecular(_kspecular);
//	renderer.setShine(shine);
//	objItem.setCalcNormals(useNormals);
//	renderer.setColors(_numOfColors);
//	renderer.setCartoon(_cartoonize);
//	objItem.setRenderType(renderMode);
//	objItem.draw(renderer);
//	*/
//}
void MeshModel::setObjectTransform(mat4 worldTransform)
{
	_world_transform = worldTransform;
}
mat4 MeshModel::getObjectTransform()
{
	return _world_transform;
}
vec3 MeshModel::getModelCenter()
{
	return vec3(_world_transform[0][3],_world_transform[1][3],_world_transform[2][3]);
}

void MeshModel::scale(const vec3& scaler) {
	_world_transform = _world_transform * Scale(scaler);
}

void MeshModel::rotate(const vec3& rotors) {
	_world_transform = _world_transform * RotateX(rotors.x) * RotateY(rotors.y) * RotateZ(rotors.z);
}

void MeshModel::setShader(Shader* shader) {
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

void MeshModel::generateBuffers() {
	
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
	
	//glBindBuffer(GL_ARRAY_BUFFER, VBOs["vtexture"]);
	//glBufferData( GL_ARRAY_BUFFER, sizeof(vec2)*sizeof(*_vtArray), _vtArray, GL_STATIC_DRAW );

}

void MeshModel::buildVAO() { // happens per Shader. VAO per (Model, Shader) pair
	if(_shader == NULL)
		throw exception();

	_vao = _shader->buildVAO(VBOs);
}

void MeshModel::copyData()
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

void MeshModel::loadBuffers() { // Load buffers into GPU

}

void MeshModel::bindBuffers() { // Make buffers active

}