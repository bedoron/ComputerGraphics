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
	vec3 _color;
	vec3 _kDiffuze;
	vec3 _kAmbiant;
	vec3 _kspecular;
	GLfloat shine;
	int _numOfColors;
	bool _cartoonize;
public:
	
	MeshModel(OBJItem item);
	~MeshModel(void);
	void loadFile(string item);
	void draw(Renderer& renderer);
	void setObjectTransform(mat4 worldTransform);
	mat4 getObjectTransform();
	vec3 getModelCenter();
	bool useNormals;
	void scale(const vec3& scaler);
	void rotate(const vec3& rotors);

	void setDrawBox(bool val) { objItem.setDrawBox(val); } // Bummer
	void setVertexNormal(bool val){ objItem.setDrawVertexNormal(val);}
	bool getNormal() {return objItem.getNormal();}
	void setNormal(bool normal) { objItem.setNormal(normal);}
	bool getVertexNormal() {return objItem.getVertexNormal();}
	bool getDrawBox() { return objItem.getDrawBox(); }
	void setColor(vec3 color){_color = color;}
	void setKAbmbiant(vec3 kambiant){_kAmbiant=kambiant;}
	void setKDiffuze(vec3 kdiffuze){_kDiffuze=kdiffuze;}
	void setKspecular(vec3 kspecular){_kspecular=kspecular;}
	void setShininess(GLfloat s){shine = s;}
	void setVNormal(bool nrml){useNormals = nrml;}
	void setColor(int colors){_numOfColors = colors;}
	void setCartoonize(bool val){_cartoonize=val;}

};
