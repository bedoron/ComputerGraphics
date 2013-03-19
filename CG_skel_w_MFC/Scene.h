#pragma once

#include "gl/glew.h"
#include <vector>
#include <string>
//#include "Renderer.h"
#include "Utils.h"
#include "Camera.h"
#include "Light.h"
#include "LightsUBO.h"

#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include <list>
#include <map>
#include <deque>


using namespace std;
enum ActiveEntity_t { WORLD_ACTIVE, MODEL_ACTIVE, CAMERA_ACTIVE };
class CModelData;

class Scene {

	vector<Model*> models;
	vector<Light*> lights;
	vector<Camera*> cameras;
	Light* m_activeLight;
	Camera*	m_activeCamera;
	Renderer *m_renderer;
	OBJItem item;
	GLfloat size;
	Model* axes;
	vector<string> names;
	vector<string> cameraNames;
	ActiveEntity_t activeEntity;
	bool modelFrameOps; // Decide if manipulation is done with respect to model space or world space
	CModelData *model_win;
	enum{ortho=3000,frusum,prespective};
	int currentView ;
	int m_activeModel;
	GLfloat _left,_right,_top,_bottom,_zfar,_znear,_fovy,_aspect;
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
	void setFrustum(float left,float right,float top,float down,float zNear,float zFar);
	void setOrtho(float left,float right,float top,float down,float zNear,float zFar);
	void setPrespective(float fovy,float aspect,float znear, float zfar);
	void removeActiveModel();
	void removeActiveCamera();
	void setZoom(GLfloat zoom);
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
	
};