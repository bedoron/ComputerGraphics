#pragma once

#include "gl/glew.h"
#include <vector>
#include <string>
#include "Renderer.h"
#include "Utils.h"
#include "Camera.h"
#include "Light.h"
using namespace std;
enum ActiveEntity_t { WORLD_ACTIVE, MODEL_ACTIVE, CAMERA_ACTIVE };
class CModelData;

class Model {
protected:
	string name;
	virtual ~Model() {}
public:
	void virtual draw(Renderer& renderer)=0;
	void virtual setObjectTransform(mat4 worldTransform)=0;
	mat4 virtual getObjectTransform()=0;
	vec3 virtual getModelCenter()=0;
	const string& getName() const { return name; };
	const string& setName(const string &newName) { name = newName; return name; };
};

//

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
	int _width,_height;
public:
	Scene();

	Scene(Renderer *renderer); 
	Scene(Renderer *renderer, CModelData& win);

	void initDefaultCamera();
	void initDefaultLight();
	void loadOBJModel(string fileName,string id);

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
	void addLight(Light* newLight);
	void changeLightDirection(mat4 rotation);
	void changeLightLocation(mat4 rotation);
	void setFog(vec3 fogColor,GLfloat density);
};