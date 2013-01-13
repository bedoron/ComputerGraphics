#pragma once
#include <vector>
#include "Face.h"
#include <exception>
#include "Renderer.h"
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
	GLuint _buffer[6];
	GLuint _vao;

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
	vec3& getVertexByNumber(int id);
	vec3& getNormalByNumber(int id);
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
	int getVNsize()
	{
		return normals.size();
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
	void setKvalue(vec3 kambiant,vec3 kdiffuse,vec3 kspecular,GLfloat shine)
	{
		_kspecular=kspecular;
		_kDiffuze = kdiffuse;
		_kAmbiant = kambiant;
		_shine = shine;
	}
};

