#pragma once
#include "scene.h"
#include "vec.h"
#include "mat.h"
#include <string>
//#include "Renderer.h"
#include "Utils.h"
#include <map>
#include "Shader.h"
#include "Texture.h"

using namespace std;

class Model {
//	GLuint vaoID;
//	GLuint vboID[7];
protected :
	Model() {}

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
	int renderMode;

	vec4 *verticesArray4;
	vec4 *normalsArray4;
	vec4 *_kAmbiantArray;
	vec4 *_kDifuseArray;
	vec4 *_kSpecularArray;
	float *_shineArray;
	vec2 *_vtArray;
	//vec3* normalsArray; // ?!

	map<string, GLuint> VBOs;
	map<string, pair<Shader*, GLuint>> vaos; // Shader name --> (Shader itself, Vao mapping)
	map<string, Texture*> textures; // Shader sampler name --> Texture to use with this sampler

	GLuint _vao;
	Shader *_shader;
	string name;

	void copyData(); // flatten all data structures
	void generateBuffers(); // generate VBOs and bind copied data
	

	void loadBuffers();
	void bindBuffers();
	
	void buildVAO();
public:
	const string& getName() const { return name; };
	const string& setName(const string &newName) { name = newName; return name; };

	void setShader(Shader* shader);
	void setTexture(const string &sampler, Texture* texture);
	const map<string, Texture*>& getTextures();

	Model(OBJItem item);
	~Model(void);
	void loadFile(string item);
	void draw(Shader *shader = 0);
	void setObjectTransform(mat4 worldTransform);
	mat4 getObjectTransform();
	vec3 getModelCenter();
	bool useNormals;
	void scale(const vec3& scaler);
	void rotate(const vec3& rotors);
	GLuint getVAO();

// These needs to be revised
	int getNumOfVertices() { return objItem.faces.size() * 3; }
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
	void setRenderType(int val){renderMode = val;}

};
