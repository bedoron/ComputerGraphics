#include "stdafx.h"
#include "Scene.h"
#include "ShaderTexture.h"
#include <string>
#include "Utils.h"
#include "ModelData.h"
#include "ShadersDB.h"
#include <cstring>
#include "resource.h"

using namespace std;

Model* Scene::loadOBJModel(string fileName,string id)
{
	try
	{
		item = Utils::getInstance().parseOBJ(fileName,id);
	}
	catch(ObjParserFileNotFound &err)
	{
		cerr << err.what()<<endl;
		throw err;
	}
	names.push_back(id);
	Model *model = new Model(item);

	model->setShader(shader);
	models.push_back(model);
	model->setName(id);
	setActiveModel(models.size()-1);
	return model;
}


void Scene::initDefaultCamera() {
	_renderCamera=FALSE	;
	m_activeCamera = new Camera();
	Camera *camera2 = new Camera();
	Camera *camera3 = new Camera();
	Camera *camera4 = new Camera();
	cameras.push_back(m_activeCamera); // Default camera for debug
	cameras.push_back(camera2);
	cameras.push_back(camera3);
	cameras.push_back(camera4);
	m_activeCamera->LookAt(vec4(5,5,5,1),vec4(0,0,0,1),vec4(0,1,0,1));	
	camera2->LookAt(vec4(-5,5,5,0), vec4(0,0,0,1), vec4(0,1,0,1));
	camera3->LookAt(vec4(-5,5,-5,0), vec4(0,0,0,1), vec4(0,1,0,1));
	camera4->LookAt(vec4(5,5,-5,0), vec4(0,0,0,1), vec4(0,1,0,1));
	_left = _bottom = _znear = 1;
	_right = _top = -5;
	_zfar = 50;
	_fovy = 45;
	_aspect=1;
	m_activeCamera->Perspective(45,1,_znear,_zfar);
	camera2->Perspective(45,1,_znear,_zfar);
	camera3->Perspective(45,1,_znear,_zfar);
	camera4->Perspective(45,1,_znear,_zfar);
	shader->setProjection(m_activeCamera->getProjection());
	
	currentView = prespective;
	setActiveCamera(1);
}
void Scene::initDefaultLight()
{
	m_activeLight = new Light(vec4(1,1,1,0), vec4(0.2,0.2,0.2,1));
	activeLight=m_activeLight; // W00t ?!
	lights.push_back(m_activeLight);
	updateShadersLight();
}


void Scene::updateShadersLight() {
	map<string, Shader*>::iterator sit = shaders.begin();
	for(;sit != shaders.end(); ++sit) {
		*(sit->second) << lights;
		sit->second->setGlobalAmbience(globalAmbience);
	}
}

void Scene::setGlobalAmbience(vec4 ambient) {
	globalAmbience = ambient;
	updateShadersLight();
	draw();
}

vec4 Scene::getGlobalAmbience() {
	return globalAmbience;
}

void Scene::initShaders() {
	INIT_SHADERS_DB(database); // if you are Arik, Dont touch this !

	Shader *tmp;
	for(s_shader_entry *entry = database; strcmp(entry->name, "")!=0; ++entry) {
		if(entry->hasTexture) {
			ShaderTexture *ttmp = new ShaderTexture(entry->name, entry->vertex_shader_file_name, entry->fragment_shader_file_name, textures);
			const char **samplers = &(entry->smapler0);
			for(int i=0; i < MAX_SAMPLERS; ++i) { // Initialize samplers by name
				const char *sampler = *(samplers + i);
				if(sampler!=NULL)
					ttmp->setTextureRealName(sampler);
			}
			// Attach default shaders from entry->smaplers[]
			tmp = ttmp;
		} else 
			tmp = new Shader(entry->name, entry->vertex_shader_file_name, entry->fragment_shader_file_name);
		tmp->checkError();
		tmp->loadProgram();
		tmp->checkError();
		shaders[tmp->getName()] = tmp;
	}
	// Create default shader
	shader = tmp;

	updateShadersLight();

	shader->bind();
	shader->checkError();
}

void Scene::reloadShaders() {
	map<string, Shader*>::iterator sit = shaders.begin();
	vector<Model*>::iterator mit = models.begin();

	/*for(;mit != models.end(); ++mit) {
		(*mit)->invalidateVAOs();
	}*/

	for(;sit != shaders.end(); ++sit) {
		sit->second->reload();
		sit->second->setCameraParams(m_activeCamera);
		updateShadersLight();
	}

	mit = models.begin();
	for(;mit != models.end(); ++mit) {
		//(*mit)->setShader((*mit)->getShader()); // Reboot shader
		(*mit)->restoreDefaults(); // Reboot shader
	}



	draw();
}


void Scene::initTextures() {
	INIT_TEXTURES_DB(database); // if you are Arik, Dont touch this !

	Texture *texture;
	for(const char **entry = database; strcmp(*entry, ""); ++entry) {
		string name(*entry);
		texture = new Texture(name);
		textures[texture->getName()] = texture;
	}
	updateModelWinTex();
}

void Scene::addTexture(const string &file, const string& path) {
	Texture *texture = new Texture(file, path);
	if(textures.find(texture->getName()) != textures.end()) {
		Texture *old = textures[texture->getName()];
		delete old;
	}
	textures[texture->getName()] = texture;
	updateModelWinTex();
}

void Scene::updateModelWinTex() {
	vector<const string> texNames;
	for(map<string, Texture*>::const_iterator it = textures.begin(); it != textures.end(); ++it)
		texNames.push_back(it->first);
	model_win->updateTextures(texNames);
}

void Scene::updateModelSampler(Model *model, const string &samplerName, const string &textureName) {
	map<string, Texture*>::const_iterator it = textures.find(textureName);
	if(it == textures.end()) {
		cerr << "Invalid texture selected\n";
		return;
	}
	Texture *texture = it->second;
	Shader *shader = model->getShader();
	//string samplerAppName = shader->translateSamplerName(samplerName);
	// samplerName is already in app name
	model->setTexture(samplerName, texture);
	draw();
}

void Scene::updateModelSampler(const string &samplerName, const string &textureName) {
	Model *model = getActiveModel();
	if(model == NULL) {
		cerr << "Cant change sampler settings for active model as there is no active model\n";
		return;
	}

	updateModelSampler(model, samplerName, textureName);
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
	initTextures();
	initShaders(); // None will work without this
	initDefaultCamera();
	initDefaultLight();
	moveInterval=1;
}

Scene::Scene():models(),cameras(), axes(0), activeEntity(WORLD_ACTIVE), model_win(0),lights(),_addFog(false),animate(false)
{
	initHook();
}
Scene::Scene(Renderer *renderer) : m_renderer(renderer),names(),m_activeModel(-1),size(0),models(),cameras(),
	activeCamera(0), axes(0), activeEntity(WORLD_ACTIVE), model_win(0),lights(),_addFog(false),animate(false)
{
	initHook();
};

Scene::Scene(/*Renderer *renderer, */ CModelData& win) : m_renderer(0),names(),m_activeModel(-1),size(0),models(),cameras(),
	activeCamera(0), axes(0), activeEntity(WORLD_ACTIVE), model_win(&win),lights(),_addFog(false),animate(false)
{
	initHook();
	model_win->setScene(this);
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
		(*it)->draw(animate);
	}
	glFinish();
	refreshModelWindow(); // IMPORTANT HOOK !
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
		it->second->checkError();
		it->second->setCameraParams(camera);
		it->second->checkError();
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
	draw();
	_left= left;
	_right = right;
	_top= top;
	_bottom = down;
	_znear = zNear;
	_zfar= zFar;
	currentView = frusum;
}
void Scene::setOrtho(float left,float right,float top,float down,float zNear,float zFar)
{
	m_activeCamera->Ortho(left,right,down,top,zNear,zFar);
	shader->setCameraParams(m_activeCamera);
	currentView = ortho;
	_left= left;
	_right = right;
	_top= top;
	_bottom = down;
	_znear = zNear;
	_zfar= zFar;
	draw();
}
void Scene::setPrespective(float fovy,float aspect,float znear, float zfar)
{
	m_activeCamera->Perspective(fovy,aspect,znear,zfar);
	shader->setCameraParams(m_activeCamera);
	currentView= prespective;
	_fovy = fovy;
	_aspect = aspect;
	_znear = znear;
	_zfar= zfar;
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
	if(name[0] == ' ') {
		string tmp = name.substr(2);
		return shaders[name.substr(2)];
	}
	return shaders[name];
}

void Scene::addCube() {
	throw exception("Not implemented!");
}


vector<string> Scene::getLights() {
	vector<string> lnames;
	char tmp_name[50];
	for(int i = 0; i < lights.size(); ++i) {
		
		sprintf(tmp_name, "(%.0f,%.0f,%.0f,%.0f)c:(%.2f,%.2f,%.2f)", 
			lights[i]->position.x, lights[i]->position.y, lights[i]->position.z, lights[i]->position.w, 
			lights[i]->color.x, lights[i]->color.y, lights[i]->color.z);   

		string strName(tmp_name);

		if(lights[i] == activeLight) 
			strName = "*" + strName;
		else 
			strName = " " + strName;

		lnames.push_back(strName);
	}
		
	return lnames;

}

void Scene::addLight(Light* newLight)
{
	lights.push_back(newLight);
	activeLight = newLight;
	updateShadersLight();
	draw();
}

void Scene::removeActiveLight() {
	vector<Light*>::iterator it = lights.begin();
	for(; it != lights.end(); ++it) {
		if((*it) == activeLight) break;
	}
	if(it==lights.end()) {
		cerr << "No active light to remove found, set one active\n";
		return;
	}
	lights.erase(it);
	activeLight = NULL;
	updateShadersLight();

	draw();
}

void Scene::setActiveLight(int index) {
	if(index < 0 || index >= lights.size())
		throw exception("Invalid set active light index");

	activeLight = lights[index];
}

void Scene::changeLightDirection(mat4 rotation)
{
	if(activeLight == NULL) return;
	activeLight->position = rotation * activeLight->position;
	updateShadersLight();
	draw();
	cerr << " light direction is " << activeLight->position << "\n";
}
void Scene::changeLightLocation(mat4 rotation)
{
	if(activeLight == NULL) return;
	activeLight->position = rotation * activeLight->position;
	updateShadersLight();
	draw();
	cerr << " light location is " << activeLight->position << "\n";
}

void Scene::setFog(vec3 fogColor,GLfloat density)
{

}