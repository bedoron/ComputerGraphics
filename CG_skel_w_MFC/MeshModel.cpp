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
	
}

MeshModel::~MeshModel(void)
{
}

void MeshModel::draw(Renderer& renderer)
{
	renderer.SetObjectMatrices(_world_transform);
	objItem.setColor(_color);
	renderer.setKAbmbiant(_kAmbiant);
	renderer.setKDiffuze(_kDiffuze);
	renderer.setKspecular(_kspecular);
	renderer.setShine(shine);
	objItem.setCalcNormals(useNormals);
	renderer.setColors(_numOfColors);
	renderer.setCartoon(_cartoonize);
	objItem.setRenderType(renderMode);
	objItem.draw(renderer);
}
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