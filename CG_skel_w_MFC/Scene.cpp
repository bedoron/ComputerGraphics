#include "stdafx.h"
#include "Scene.h"
#include "MeshModel.h"
#include <string>
#include "Utils.h"
#include "CubeModel.h"
#include "AxesModel.h"
#include "ModelData.h"
using namespace std;

void Scene::loadOBJModel(string fileName,string id)
{
	try
	{
		item = Utils::getInstance().parseOBJ(fileName,id);
	}
	catch(ObjParserFileNotFound &err)
	{
		cerr << err.what()<<endl;
	}
	names.push_back(id);
	MeshModel *model = new MeshModel(item);

	model->setShader(shader);
	models.push_back(model);
	model->setName(id);
	setActiveModel(models.size()-1);
}

void Scene::initDefaultCamera() {
	_renderCamera=FALSE	;
	m_activeCamera = new Camera();
	cameras.push_back(m_activeCamera); // Default camera for debug
	m_activeCamera->LookAt(vec4(5,5,5,1),vec4(0,0,0,1),vec4(0,1,0,1));	
	_left = _bottom = _znear = 1;
	_right = _top = -5;
	_zfar = 50;
	_fovy = 45;
	_aspect=1;
	m_activeCamera->Perspective(45,1,_znear,_zfar);
	//m_renderer->SetProjection(m_activeCamera->getProjection());
	shader->setProjection(m_activeCamera->getProjection());
	
	//_height = m_renderer->getHeight();
	//_width = m_renderer->getWidth();
	currentView = prespective;
	setActiveCamera(1);
}
void Scene::initDefaultLight()
{
	m_activeLight = new Light();
	
	m_activeLight->setDirection(vec3(1,1,1));
	m_activeLight->setLightType(L_PARALEL);
	m_activeLight->setIntencity(vec3(255,255,255));
	lights.push_back(m_activeLight);
	
	//m_renderer->addLight(m_activeLight);
	
	

	activeLight=m_activeLight;
	
}

void Scene::initShaders() {
	const char *shader_files[] = { "PhongVshader.glsl", "PhongFshader.glsl", "ToonVshader.glsl" , "ToonFshader.glsl"};
//	const char *shader_files[] = { "PhongVshader.glsl", "PhongFshader.glsl"};
	const int total = 2;
	Shader *tmp;
	for(int i=0; i<total; ++i) {
		tmp = new Shader(shader_files[2*i], shader_files[2*i+1]);
		tmp->loadProgram();
		shaders[tmp->getName()] = tmp;
	}
	shader = tmp;
//	shader->loadProgram();
	shader->bind(); // Activate first shader
}

vector<string> Scene::listShaders() {
	map<string,Shader*>::const_iterator it = shaders.begin();
	vector<string> shadersQ;
	for(;it!=shaders.end(); ++it) {
		string name = (*it).first;
		if(name == shader->getName())
			name = "  " + name;
		shadersQ.push_back(name);
	}
	return shadersQ;
}

void Scene::initHook() {
	initShaders(); // None will work without this
	initDefaultCamera();
//	initDefaultLight();
	moveInterval=1;
}

Scene::Scene():models(),cameras(), axes(0), activeEntity(WORLD_ACTIVE), model_win(0),lights(),_addFog(false)
{
	initHook();
}
Scene::Scene(Renderer *renderer) : m_renderer(renderer),names(),m_activeModel(-1),size(0),models(),cameras(),
	activeCamera(0), axes(0), activeEntity(WORLD_ACTIVE), model_win(0),lights(),_addFog(false)
{
	initHook();
};

Scene::Scene(/*Renderer *renderer, */ CModelData& win) : m_renderer(0),names(),m_activeModel(-1),size(0),models(),cameras(),
	activeCamera(0), axes(0), activeEntity(WORLD_ACTIVE), model_win(&win),lights(),_addFog(false)
{
//	model_win->setScene(this); -- This should be revised because it uses the renderer :(
	initHook();
};

void Scene::refreshModelWindow() {
	if(0==models.size() || m_activeModel == -1) return; // stop crashing, Bitch
	Model *active = models[m_activeModel];
	if(model_win!=0) {
		model_win->setModel(active);
		if(model_win->IsWindowVisible())
			model_win->refreshModelData();
	}
}

void Scene::hideModelWindow() {
	if(model_win!=0)
		model_win->ShowWindow(SW_HIDE);
}

void Scene::showModelWindow() {
	if(model_win!=0)
		model_win->ShowWindow(SW_SHOW);
}

Model* Scene::setActiveModel(int id) {
	if(id >= models.size()) return 0;
	m_activeModel = id;
	activateModel();
	Model *active = models[m_activeModel];
	refreshModelWindow();
	
	cerr << active->getName() << "\n" << active->getObjectTransform() << "\n";
//	cerr << "Center transformed to :\n" << m_renderer->calculateMvpPCTransformation(vec4(active->getModelCenter())) << "\n";
	return active;
}

Model* Scene::getActiveModel() {
	if(0 == models.size() || m_activeModel == -1) return 0;
	return models[m_activeModel];
}

void Scene::draw(vec3 rotation)
{
	mat4 rotate = RotateX(rotation.x)*RotateY(rotation.y)*RotateZ(rotation.z);
	draw(rotation);
}

void Scene::draw(vec3 rotation,GLfloat zoom,bool change)
{
	std::cerr << "Scene::draw(vec3 rotation,GLfloat zoom,bool change) deprecated\n";
}

void Scene::drawDebug(vec4 eye) {
	std::cerr << "Narf!\n";
};


void Scene::draw(mat4 translation)
{
	if(models.size()!=0 && m_activeModel!=-1) {
		mat4 oTransform =models[m_activeModel]->getObjectTransform() ;	
		oTransform =translation *  oTransform ; 
		models[m_activeModel]->setObjectTransform(oTransform);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//shader->setCameraParams(m_activeCamera);
	for(std::vector<Model*>::iterator it = models.begin(); it != models.end(); ++it)  //2
	{
		(*it)->draw(shader);
	}

	//refreshModelWindow(); // IMPORTANT HOOK !
}

Model* Scene::nextModel() {
	return setActiveModel((m_activeModel + 1)%models.size());
}

void Scene::drawDemo()
{
	throw string("Nigger");
}

GLfloat Scene::getSize()
{
	return size;
}
void Scene::setNormal(bool normal)
{
	throw string("Not implemented");
	// Iterate all elements and tell them to draw their normals
}

void Scene::clearScene()
{
	names.clear();
	models.clear();
	model_win->ShowWindow(SW_HIDE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // TODO: Check if its enough
}

int Scene::getModelCount()
{
	return models.size();
}

std::vector<string> Scene::getNames()
{
	return names;
}

std::vector<string> Scene::getCameraNames() {
	cameraNames.clear();
	vector<Camera*>::const_iterator it = cameras.begin();
	for(;it!=cameras.end();++it) {
		cameraNames.push_back((*it)->getName());
	}
	return cameraNames;
}

void Scene::refreshActiveCamera() {
	updateCameraInAllShaders(cameras[activeCamera]);
	draw(mat4(1));
}

void Scene::pointCameraAt()
{
	cameras[activeCamera]->pointCameraAt(vec4(models[m_activeModel]->getModelCenter()));
	
	updateCameraInAllShaders(cameras[activeCamera]);
	
	draw(mat4(1));
}

bool Scene::setActiveCamera(int num)
{
	if(cameras.size()< num) 
		return false;
	else
		activeCamera = num-1;
	m_activeCamera = cameras[activeCamera];
	std::cerr << "camera number "<< num << " was selected\n";

	// Set active camera for all shaders
	updateCameraInAllShaders(cameras[activeCamera]);

//	shader->setCameraParams(cameras[activeCamera]); // Set projection + Transformation

	draw(mat4(1));
	return true;
}

void Scene::updateCameraInAllShaders(Camera *camera) {
	map<string, Shader*>::iterator it;
	for(it = shaders.begin(); it != shaders.end(); ++it) {
		it->second->setCameraParams(camera);
	}
}


void Scene::addCamera(vec4 eye,vec4 at,vec4 up)
{
	Camera* newCamera = new Camera();
	newCamera->LookAt(eye,at,up);
	cameras.push_back(newCamera);
	newCamera->setProjection( cameras[activeCamera]->getProjection() ); // moo
	activeCamera = cameras.size();
	if (!setActiveCamera(activeCamera)) {
		std::cerr << "Failed setting active camera " << activeCamera << "\n";
	}

	
}
void Scene::drawCameras()
{
	/*
	m_renderer->SetObjectMatrices(mat4(1));
	for(std::vector<Camera*>::iterator it = cameras.begin(); it != cameras.end(); ++it) 
	{
		(*it)->draw(*m_renderer);
	}
	*/
	throw string("never ever call this piece of crap");
}
void Scene::setFrustum(float left,float right,float top,float down,float zNear,float zFar)
{
	m_activeCamera->Frustum(left,right,down,top,zNear,zFar);
	shader->setCameraParams(m_activeCamera);
	//m_renderer->SetProjection(m_activeCamera->getProjection());
	//m_renderer->SetCameraTransform(m_activeCamera->getInverseTransformation());
	draw();
	_left= left;
	_right = right;
	_top= top;
	_bottom = down;
	_znear = zNear;
	_zfar= zFar;
	currentView = frusum;
//	m_renderer->setZdistance(abs(_znear-_zfar)); // WHY ?!
//	m_renderer->setZFar(_zfar);// WHY ?!
//	m_renderer->setZNear(_znear);// WHY ?!
}
void Scene::setOrtho(float left,float right,float top,float down,float zNear,float zFar)
{
	m_activeCamera->Ortho(left,right,down,top,zNear,zFar);
	shader->setCameraParams(m_activeCamera);
	//m_renderer->SetProjection(m_activeCamera->getProjection());
	//m_renderer->SetCameraTransform(m_activeCamera->getInverseTransformation());
	currentView = ortho;
	_left= left;
	_right = right;
	_top= top;
	_bottom = down;
	_znear = zNear;
	_zfar= zFar;
	//m_renderer->setZdistance(abs(_znear-_zfar));
	//m_renderer->setZFar(_zfar);
	//m_renderer->setZNear(_znear);
	draw();
}
void Scene::setPrespective(float fovy,float aspect,float znear, float zfar)
{
	m_activeCamera->Perspective(fovy,aspect,znear,zfar);
	shader->setCameraParams(m_activeCamera);
	//m_renderer->SetProjection(m_activeCamera->getProjection());
	//m_renderer->SetCameraTransform(m_activeCamera->getInverseTransformation());
	currentView= prespective;
	_fovy = fovy;
	_aspect = aspect;
	_znear = znear;
	_zfar= zfar;
	//m_renderer->setZdistance(abs(_znear-_zfar));
	//m_renderer->setZFar(_zfar);
	//m_renderer->setZNear(_znear);
	draw();
}

void Scene::removeActiveModel() {
	Model *active = getActiveModel();
	if(!active) return; 
	models.erase(models.begin()+m_activeModel);
	names.erase(names.begin()+m_activeModel);
	m_activeModel = -1;
	hideModelWindow();
	draw();
}

void Scene::removeActiveCamera() {
	throw string("Not implemented :(");
}


void Scene::setRenderer(int width , int height)
{
	return; // Do nothing for now

	//mat4 Projection = shader->getProjection();
	//int oldWidth = shader->getWidth();
	//int oldHeight = shader->getHeight();	
	////mat4 Projection = m_renderer->getProjection();
	////int oldWidth = m_renderer->getWidth();
	////int oldHeight = m_renderer->getHeight();
	//GLfloat dx = width-oldWidth;
	//GLfloat dy = height - oldHeight;
	//GLfloat dxPercent = (GLfloat)(abs(((GLfloat)dx)/((GLfloat)oldWidth)));
	//GLfloat dyPercent = (GLfloat)(abs(((GLfloat)dy)/((GLfloat)oldHeight)));
	//GLfloat newAspect = (GLfloat)((GLfloat)width)/((GLfloat)height);
	//
	//delete(m_renderer);
	//m_renderer = new Renderer(width,height);
	//m_renderer->addLights(lights);
	//m_renderer->setActiveCamera(m_activeCamera);
	//if(dx>0) // X got bigger
	//{
	//	GLfloat deltaViewX = (_right-_left)*dxPercent;
	//	GLfloat halfDeltaViewX = deltaViewX/2;
	//	_left -= halfDeltaViewX;
	//	_right += halfDeltaViewX;
	//}
	//if (dx < 0)
	//{
	//	GLfloat deltaViewX = (_right-_left)*dxPercent;
	//	GLfloat halfDeltaViewX = deltaViewX/2;
	//	_left += halfDeltaViewX;
	//	_right -= halfDeltaViewX;
	//}
	//if (dy > 0)
	//{
	//	GLfloat oldViewY = _top-_bottom;
	//	GLfloat deltaViewY = ( _top-_bottom)*dyPercent;
	//	GLfloat halfDeltaViewY = deltaViewY/2;
	//	_bottom -= halfDeltaViewY;
	//	_top += halfDeltaViewY;
	//}

	//if (dy < 0)
	//{
	//	GLfloat oldViewY = _top-_bottom;
	//	GLfloat deltaViewY = ( _top-_bottom)*dyPercent;
	//	GLfloat halfDeltaViewY = deltaViewY/2;
	//	_bottom += halfDeltaViewY;
	//	_top -= halfDeltaViewY;
	//}
	//switch(currentView)
	//{
	//case ortho:
	//	{
	//		setOrtho(_left,_right,_top,_bottom, _znear, _zfar);
	//		break;
	//	}
	//case frusum:
	//	{
	//		setFrustum(_left,_right,_top,_bottom, _znear, _zfar);
	//		break;
	//	}
	//case prespective:
	//	{
	//		setPrespective(_fovy,newAspect,_znear,_zfar);
	//		break;
	//	}
	//}
}
void Scene::setZoom(GLfloat zoom)
{
	switch (currentView)
	{
	case frusum:
		{
			_left -= zoom;
			_right += zoom;
			_top += zoom;
			_bottom -= zoom;
			setFrustum(_left,_right,_top,_bottom, _znear, _zfar);
			break;
		}
	case ortho:
		{
			_left -= zoom;
			_right += zoom;
			_top += zoom;
			_bottom -= zoom;
			setOrtho(_left,_right,_top,_bottom, _znear, _zfar);
			break;
		}
	case prespective:
		{
			_fovy -= 3*zoom;
			setPrespective(_fovy,_aspect,_znear,_zfar);
			break;
		}
	}
}

Shader* Scene::getShader(const string& name) {
	//shaders.at(name);
	return shaders[name];
}

void Scene::addCube()
{
	throw string("Not implemented!");
/*	Model* cube = new CubeModel(-1,1,-1,1,-1,1); -- Changed function in Model class. need to add it here too
	models.push_back(cube);
	names.push_back("Cube");
	setActiveModel(models.size()-1);*/
}
void Scene::addLight(Light* newLight)
{
	//lights.push_back(newLight);
	//m_renderer->addLights(lights);
	//activeLight = newLight;
	//draw();
}
void Scene::changeLightDirection(mat4 rotation)
{
	//vec4 newDirection = rotation*activeLight->getDirection();
	//activeLight->setDirection(vec3(newDirection.x,newDirection.y,newDirection.z));
	//cerr << " light direction is " << activeLight->getDirection() << "\n";
	//draw();
}
void Scene::changeLightLocation(mat4 rotation)
{
	//vec4 newLocation = rotation*activeLight->getLocation();
	//activeLight->setLocation(vec3(newLocation.x,newLocation.y,newLocation.z));
	//cerr << " light location is " << activeLight->getLocation() << "\n";
}
void Scene::setFog(vec3 fogColor,GLfloat density)
{
	//if(density)
	//	m_renderer->setFog(fogColor,density);
	//else
	//	m_renderer->clearFog();
	//_addFog = density !=0;
	//draw();
}