#pragma once
#include <vector>
#include "Face.h"
#include <exception>
#include "AvlTree.h"
#include "Vertex.h"
using std::out_of_range;
using namespace std;


class OBJItem
{
private:
	GLfloat minX;
	GLfloat minY;
	GLfloat minZ;
	GLfloat maxX;
	GLfloat maxY;
	GLfloat maxZ;
	vector<vec3> vertices;
	vector<Face> faces;
	vector<vec3> normals;
	vector<vec2> vTex;
	bool drawBox;
	bool drawVertexNormal;
	bool drawNormal;
	bool drawFaces;
	AvlTree<Vertex>* verticesTree;
	vec3 _color;
	bool calcNormals;
	int renderMode;

	vec3 _kDiffuze;
	vec3 _kAmbiant;
	vec3 _kspecular;
	GLfloat _shine;
	//OpenGL
	vec3* verticesArray;
	vec4* verticesArray4;
	vec3* normalsArray;
	vec4* normalsArray4;
	vec4* _kAmbiantArray;
	vec4* _kDifuseArray;
	vec4* _kSpecularArray;
	float* _shineArray;
	vec2* _vtArray;

	GLuint _vao;
	mat4 _world_transform;
	GLuint GLkAmbient;
	GLuint GLkDiffuse;
	GLuint GLkSpecular;
	GLuint GLShininess;


public:
	OBJItem(void);
	OBJItem(const OBJItem& item);
	~OBJItem(void);
	void addVertex(vec3& vertex);
	void addFace(Face& face);
	void addNormal(vec3& normal);
	void addVT(vec2 vt);
	vec3& getVertexByNumber(int id);
	vec3& getNormalByNumber(int id);
	vec2& getVTByNumber(int id);
	void draw(Renderer& renderer);
	void draw(GLuint program);
	GLfloat getMinX();
	GLfloat getMinY();
	GLfloat getMinZ();
	GLfloat getWidth();
	GLfloat getHeight();
	int getVertexSize()
	{
		return vertices.size();
	}
	int getFacesSize()
	{
		return faces.size();
	}
	vec4* getFacesAsArray()
	{
		return verticesArray4;
	}
	vec4* getNormalsAsArray()
	{
		return normalsArray4;
	}
	int getVNsize()
	{
		return normals.size();
	}
	int getVTsize()
	{
		return vTex.size();
	}
	vec3 getMidPoint();

	bool getDrawBox() { return drawBox; };
	bool getNormal() { return drawNormal;}
	void setNormal(bool val){drawNormal = val;}
	bool getVertexNormal() { return drawVertexNormal;}
	void setDrawBox(bool val) { drawBox = val; };
	void setDrawVertexNormal(bool val) { drawVertexNormal = val; };
	vec3 getCalculatedNormal(int vertex);
	void setColor(vec3 color){_color = color;}
	void setCalcNormals(bool val){calcNormals = val;}
	void setRenderType(int val){renderMode = val;}
	void copyData();
	void initVao();
	void setKvalue(vec3 kambiant,vec3 kdiffuse,vec3 kspecular,GLfloat shine,mat4 world)
	{
		_kspecular=kspecular;
		_kDiffuze = kdiffuse;
		_kAmbiant = kambiant;
		_shine = shine;
		_world_transform = world;
	}
	void reDraw(GLuint program,int type);
	void drawSilhoette();
	void drawTexture(GLuint program,GLuint textureID,GLint textid);

};

