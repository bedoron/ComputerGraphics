#pragma once

#include "gl/glew.h"
#include <vector>
#include <string>
//#include "Renderer.h"
#include "Utils.h"
#include "Camera.h"
#include "Light.h"
<<<<<<< HEAD
#include "pngLib/PngWrapper.h"
=======
#include "LightsUBO.h"

#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include <list>
#include <map>
#include <deque>


>>>>>>> refs/heads/final-hw3
using namespace std;
enum programType {program_Phong,program_NormalMapping,program_Silhouette,program_toon,program_Texture,program_enviroment};
enum ActiveEntity_t { WORLD_ACTIVE, MODEL_ACTIVE, CAMERA_ACTIVE };

class CModelData;

<<<<<<< HEAD
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
	void virtual drawEnviroment(GLuint program,GLuint enviroment,GLuint textureid) =0;
	const string& getName() const { return name; };
	const string& setName(const string &newName) { name = newName; return name; };
};

//
typedef vec4  color4;
typedef vec4  point4;

enum projectionType {ortho,prespctive,frustum};
=======
>>>>>>> refs/heads/final-hw3
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
<<<<<<< HEAD
	GLuint _texturesIds[8];
	GLint _textids[8];
	int tex_index;
public:
	Scene();
	void LookAt( const vec4& eye, const vec4& at, const vec4& up );
	void initDefaultCamera();
	void initDefaultLight();
	void loadOBJModel(MeshModel* model);
	void setProgramType(programType prog);
=======
	bool _renderCamera;
	bool _addFog;
	bool animate;
	int _width,_height;

	map<string, Shader*> shaders;
	map<string, Texture*> textures;

	Shader* shader;
	void updateCameraInAllShaders(Camera *camera);
	LightsUBO lightsUBO;

	Scene(Renderer *renderer); 
	Scene();
	void updateModelWinTex(); // This should be called each time we add textures to the system
public:
	void refreshActiveCamera();
	vector<string> listShaders();
	
	void updateModelSampler(Model *model, const string &samplerName, const string &textureName);
	void updateModelSampler(const string &samplerName, const string &textureName);

	void setAnimate(bool flag){animate = flag;}
	bool isAnimated(){return animate;}
	Scene(/*Renderer *renderer, */ CModelData& win);

	void initDefaultCamera();
	void initDefaultLight();
	void initShaders();
	void initTextures();
	void addTexture(const string &file, const string& path);

	void initHook();
	void loadOBJModel(string fileName,string id);

	void setAntialiasing(bool val) { 
		/*if(val == m_renderer->getAntialiasing()) return;
		m_renderer->setAntialiasing(val); 
		draw();*/
		// Do nothing
	}

	bool getAntialiasing() { return false; /*return m_renderer->getAntialiasing();*/ }

	void drawDebug(vec4 eye = vec4(2,5,-10,1)); //
	void draw(vec3 rotation);
	void draw(vec3 rotation,float zoom,bool update= false);
	void draw(mat4 translation = mat4(1));
	void setNormal(bool normal);
	void drawDemo();
	void clearScene();
	GLfloat getSize();
	Model* setActiveModel(int id);
	Model* getActiveModel();
	
	Light* activeLight;
	int activeCamera;
	int getModelCount();
	GLfloat moveInterval;
	void pointCameraAt();
	void setRenderer(int width , int height);
	void activateModel()	{ activeEntity =  MODEL_ACTIVE; }
	void activateCamera()	{ activeEntity = CAMERA_ACTIVE; }
	void activateWorld()	{ activeEntity = WORLD_ACTIVE;	} // Not really needed
	void modelFrame()		{ modelFrameOps = true;			}
	void worldFrame()		{ modelFrameOps = false;		}
	bool isModelFrame()		{ return modelFrameOps;			}
	Model* nextModel();	// cycle to next model
	std::vector<string> getNames();
	std::vector<string> getCameraNames();
	void addCamera(vec4 eye,vec4 at,vec4 up);
	bool setActiveCamera(int num);
	void refreshModelWindow();
	void hideModelWindow();
	void showModelWindow();
	void drawCameras();
>>>>>>> refs/heads/final-hw3
	void setFrustum(float left,float right,float top,float down,float zNear,float zFar);
	void setOrtho(float left,float right,float top,float down,float zNear,float zFar);
	void setPrespective(float fovy,float aspect,float znear, float zfar);
	void setZoom(GLfloat zoom);
<<<<<<< HEAD
	void transformActiveModel(mat4 translation);
	void draw();
	void redraw();
	void drawSilhoette();
	void loadPng(const char* fileName, const int textureUnit, const int textureNumber);
	void texture();
	void enviroment();
	void loadTexture();
	void nextTexture();
	void setTextureNum(int num);
	void background();
	void loadMarble(const int textureUnit, const int textureNumber);
=======
	void setRenderCamera(bool renderCamera) {_renderCamera = renderCamera; }
	bool getRenderCamera() {return _renderCamera; }
	Camera* getActiveCamera(){return m_activeCamera;}
	void addCube();
	void setMoveInterval(GLfloat val){moveInterval=val;}
	GLfloat getMoveInterval(){return moveInterval;}

	vector<string> getLights();
	void addLight(Light* newLight);
	void removeActiveLight();
	void setActiveLight(int index);
	void changeLightDirection(mat4 rotation);
	void changeLightLocation(mat4 rotation);

	void setFog(vec3 fogColor,GLfloat density);
	Shader* getShader(const string& name);
	void setGlobalAmbience(vec4 ambient);
	vec4 getGlobalAmbience();
>>>>>>> refs/heads/final-hw3
	
};