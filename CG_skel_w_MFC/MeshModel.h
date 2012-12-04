#pragma once
#include "scene.h"
#include "vec.h"
#include "mat.h"
#include <string>
#include "Renderer.h"
#include "Utils.h"
using namespace std;

class MeshModel : public Model
{
protected :
	MeshModel() {}
	vec3 *_vertices;
	//add more attributes
	mat4 _world_transform;
	mat3 _normal_transform;
	OBJItem objItem;

public:
	
	MeshModel(OBJItem item);
	~MeshModel(void);
	void loadFile(string item);
	void draw(Renderer& renderer);
	void setObjectTransform(mat4 worldTransform);
	mat4 getObjectTransform();
	vec3 getModelCenter();

	void scale(const vec3& scaler);
	void rotate(const vec3& rotors);

	void setDrawBox(bool val) { objItem.setDrawBox(val); } // Bummer
	void setVertexNormal(bool val){ objItem.setDrawVertexNormal(val);}
	bool getNormal() {return objItem.getNormal();}
	void setNormal(bool normal) { objItem.setNormal(normal);}
	bool getVertexNormal() {return objItem.getVertexNormal();}
	bool getDrawBox() { return objItem.getDrawBox(); }
};
