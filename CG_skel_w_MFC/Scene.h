#pragma once

#include "gl/glew.h"
#include <vector>
#include <string>
//#include "Renderer.h"
#include "Utils.h"
#include "Camera.h"
#include "Light.h"
#include "pngLib/PngWrapper.h"
using namespace std;
enum programType {program_Phong,program_NormalMapping,program_Silhouette,program_toon,program_Texture};
enum ActiveEntity_t { WORLD_ACTIVE, MODEL_ACTIVE, CAMERA_ACTIVE };

class CModelData;

class Model {
protected:
	string name;
	virtual ~Model() {}
public:
	void virtual reDraw(GLuint program,programType type)=0;
	void virtual drawSilhoette()=0;
	void virtual drawNormal(GLuint program)=0;
	void virtual setObjectTransform(mat4 worldTransform)=0;
	mat4 virtual getObjectTransform()=0;
	vec3 virtual getModelCenter()=0;
	void virtual drawTexture(GLuint program,GLuint textureID,GLint textid)=0;
	const string& getName() const { return name; };
	const string& setName(const string &newName) { name = newName; return name; };
};

//
typedef vec4  color4;
typedef vec4  point4;

enum projectionType {ortho,prespctive,frustum};
class Scene {
	bool _programChanged;
	point4 eye;
	point4 at;
	point4 up;
	mat4 _current_Projection;
	mat4 _currentCamera;
	vector<Model*> models;
	GLuint program,tex;
	MeshModel* _activeModel;
	projectionType _currentPtype;
	programType _currentProgram;
	GLfloat _left,_right,_top,_bottom,_zfar,_znear,_fovy,_aspect;
	GLuint _texturesIds[5];
	GLint _textids[5];
public:
	Scene();
	void LookAt( const vec4& eye, const vec4& at, const vec4& up );
	void initDefaultCamera();
	void initDefaultLight();
	void loadOBJModel(MeshModel* model);
	void setProgramType(programType prog);
	void setFrustum(float left,float right,float top,float down,float zNear,float zFar);
	void setOrtho(float left,float right,float top,float down,float zNear,float zFar);
	void setPrespective(float fovy,float aspect,float znear, float zfar);
	void setZoom(GLfloat zoom);
	void transformActiveModel(mat4 translation);
	void draw();
	void redraw();
	void drawSilhoette();
	void loadPng(const char* fileName, const int textureUnit, const int textureNumber);
	void texture();
	void loadTexture();
};