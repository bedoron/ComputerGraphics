#include "stdafx.h"
#include "Scene.h"
#include "ShaderTexture.h"
#include <string>
#include "Utils.h"
<<<<<<< HEAD

//#include "ModelData.h"
#include "InitShader.h"
=======
#include "ModelData.h"
#include "ShadersDB.h"
#include <cstring>
>>>>>>> refs/heads/final-hw3
using namespace std;
#define INDEX_PNG(width,x,y,c) (((y)+(x)*(width))*3+(c))

void Scene::loadOBJModel(MeshModel* model)
{
<<<<<<< HEAD
	_activeModel = model;
	models.push_back(model);
	_programChanged=true;
	redraw();
=======
	try
	{
		item = Utils::getInstance().parseOBJ(fileName,id);
	}
	catch(ObjParserFileNotFound &err)
	{
		cerr << err.what()<<endl;
	}
	names.push_back(id);
	Model *model = new Model(item);

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
	m_activeLight = new Light(vec4(1,1,1,0), vec4(1,0,0,1));
	activeLight=m_activeLight; // W00t ?!
	lights.push_back(m_activeLight);

//	lights.push_back(new Light(vec4(-1,1,1,0),vec4(0,1,0,1)));
	
	
	
	
	lightsUBO.setGlobalAmbient(vec4(0,0,0,0));
	lightsUBO.bindToPoint(Shader::UNIFORM_BINDING_POINT);

	lightsUBO << lights; // this should be called everytime we update lights
}


void Scene::setGlobalAmbience(vec4 ambient) {
	lightsUBO.setGlobalAmbient(ambient);
	draw();
}

vec4 Scene::getGlobalAmbience() {
	return lightsUBO.getGlobalAmbient();
}

void Scene::initShaders() {
	INIT_SHADERS_DB(database); // if you are Arik, Dont touch this !

	Shader *tmp;
	for(s_shader_entry *entry = database; strcmp(entry->name, "")!=0; ++entry) {
		if(entry->hasTexture) {
			ShaderTexture *ttmp = new ShaderTexture(entry->name, entry->vertex_shader_file_name, entry->fragment_shader_file_name, textures);
			const char **samplers = &(entry->smapler0);
			for(int i=0; i < MAX_SAMPLERS; ++i) {
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
	shader->bind();
	shader->checkError();
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
>>>>>>> refs/heads/final-hw3
}
void Scene::LookAt( const vec4& eye, const vec4& at, const vec4& up )
{
<<<<<<< HEAD
	vec4 zeroVec(0,0,0,0);
	vec4 tempVec = at-eye;
	if ((tempVec) == zeroVec)
	{
		_currentCamera=  mat4(1.0);
		return;
	}
	vec3 eye3(eye.x,eye.y ,eye.z);
	vec3 at3 (at.x , at.y , at.z);
	vec3 up3 (up.x , up.y , up.z);
	vec3 n = normalize(eye3-at3);   //z'
	vec3 u = normalize(cross(up3,n));  //x'

	vec3 v = normalize(cross(n,u));  //y'

	//vec4 t = vec4(0.0,0.0,0.0,1.0);
	mat3 c = transpose (mat3(u,v,n)) ;
	_currentCamera= mat4(c) * Translate(-eye);
	this->eye = eye;
=======
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
>>>>>>> refs/heads/final-hw3
}
void Scene::setProgramType(programType prog)
{
<<<<<<< HEAD
	_programChanged = true;
	_currentProgram = prog;
	redraw();
}
void Scene::initDefaultCamera() 
{
}
void Scene::initDefaultLight()
{
=======
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

	
>>>>>>> refs/heads/final-hw3
}
Scene::Scene():models(),_currentProgram(program_Phong),_programChanged(true)
{
<<<<<<< HEAD
	loadTexture();
	tex_index =0;
=======
	/*
	m_renderer->SetObjectMatrices(mat4(1));
	for(std::vector<Camera*>::iterator it = cameras.begin(); it != cameras.end(); ++it) 
	{
		(*it)->draw(*m_renderer);
	}
	*/
	throw string("never ever call this piece of crap");
>>>>>>> refs/heads/final-hw3
}
void Scene::setFrustum(float left,float right,float top,float bottom,float zNear,float zFar)
{
<<<<<<< HEAD
	_zfar=zFar;
	_znear = zNear;
=======
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
>>>>>>> refs/heads/final-hw3
	_right = right;
	_left = left;
	_top = top;
	_bottom = bottom;
	mat4 p;
	p[0][0] = 2*zNear/(right-left)	; p[0][1] = 0						; p[0][2] =  (right + left)/(right - left);	; p[0][3] = 0;
	p[1][0] = 0						; p[1][1] = 2*zNear/(top-bottom)	; p[1][2] = (top + bottom)/(top - bottom)	; p[1][3] = 0;
	p[2][0] = 0						; p[2][1] = 0						; p[2][2] = -(zFar + zNear)/(zFar - zNear)	; p[2][3] = -2*zNear*zFar/(zFar-zNear);
	p[3][0] = 0						; p[3][1] = 0						; p[3][2] = -1								; p[3][3] = 0;
	_current_Projection = p;
	_currentPtype = frustum;
}
void Scene::setOrtho(float left,float right,float top,float bottom,float zNear,float zFar)
{
	_zfar=zFar;
	_znear = zNear;
<<<<<<< HEAD
	_right = right;
	_left = left;
	_top = top;
	_bottom = bottom;
	mat4 p;
	p[0][0] = 2/(right-left); p[0][1] = 0				; p[0][2] = 0				; p[0][3] = -(right + left)/(right - left);
	p[1][0] = 0				; p[1][1] = 2/(top-bottom)	; p[1][2] = 0				; p[1][3] = -(top + bottom)/(top- bottom);
	p[2][0] = 0				; p[2][1] = 0				; p[2][2] = 2/(zNear - zFar); p[2][3] = -(zNear + zFar)/(zFar - zNear);
	p[3][0] = 0				; p[3][1] = 0				; p[3][2] = 0				; p[3][3] = 1;
	_current_Projection = p;
	_currentPtype = ortho;
}
void Scene::setPrespective(float fovy,float aspect,float zNear, float zFar)
{
	_fovy = fovy;
	_aspect = aspect;
	_zfar=zFar;
	_znear = zNear;
	GLfloat top = zNear*tan(M_PI* fovy/180);
	GLfloat right = top*aspect;
	mat4 c;
	c[0][0] = zNear/right;		c[0][1] = 0;				c[0][2] = 0;						c[0][3] = 0;
	c[1][0] = 0;				c[1][1] = zNear/top;		c[1][2] = 0;						c[1][3] = 0; 
	c[2][0] = 0;				c[2][1] = 0;				c[2][2] = (-zFar+zNear)/(zFar-zNear);c[2][3] = (-2*zFar*zNear)/(zFar-zNear); 
	c[3][0] = 0;				c[3][1] = 0;				c[3][2] = -1;						c[3][3] = 0; 
	_currentPtype = prespctive;
	_current_Projection = c;
}


=======
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
>>>>>>> refs/heads/final-hw3
void Scene::setZoom(GLfloat zoom)
{
	switch (_currentPtype)
	{
	case frustum:
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
	case prespctive:
		{
			_fovy -= 3*zoom;
			setPrespective(_fovy,_aspect,_znear,_zfar);
			break;
		}
	}
	GLuint projection = glGetUniformLocation( program, "Projection");
	glUniformMatrix4fv(projection, 1, GL_TRUE, _current_Projection);
	
}
void Scene::draw()
{
	_programChanged = false;
	GLuint vao;
	glGenVertexArrays(1 ,&vao);
	glBindVertexArray( vao );
	switch(_currentProgram)
	{
	case program_Phong:
		{
			program = InitShader( "PhongVshader.glsl","PhongFshader.glsl");
			break;
		}
	case program_NormalMapping:
		{
			program = InitShader( "NormalMapVshader.glsl","NormalMapFshader.glsl");
			texture();
			return;
		}
	case program_Silhouette:
		{
			program = InitShader( "SilhouetteShaderV.glsl","SilhouetteShaderF.glsl");
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			break;
		}
	case program_toon:
	{
		program = InitShader( "ToonVshader.glsl","ToonFshader.glsl");
		break;
	}
	case program_Texture:
	{
		program = InitShader( "TextureVshader.glsl","TextureFshader.glsl");
		texture();
		return;
	}
	case program_enviroment:
	{
		program = InitShader( "EnviromentMapVshader.glsl","EnviromentMapFshader.glsl");
		enviroment();
		return;
	}
	}

	glUseProgram( program );
	
	GLuint Camera_view = glGetUniformLocation(program,"CameraView");
	GLuint projection = glGetUniformLocation( program, "Projection");
	GLuint eyeLocation = glGetUniformLocation( program, "eye");
	glEnable(GL_DEPTH_TEST);
	
	glUniformMatrix4fv(projection, 1, GL_TRUE, _current_Projection);
	glUniformMatrix4fv(Camera_view, 1, GL_TRUE, _currentCamera);
	glUniform4f(eyeLocation,eye.x,eye.y,eye.z,eye.w);
	for(std::vector<Model*>::iterator it = models.begin(); it != models.end(); ++it)  
	{
		(*it)->drawNormal(program);
	}
	
	glClearColor(0.5,0.5,0.5,1);

	
}
void Scene::texture()
{
		
	glEnable(GL_TEXTURE_2D);
	
	GLuint Camera_view = glGetUniformLocation(program,"CameraView");
	GLuint projection = glGetUniformLocation( program, "Projection");
	GLuint eyeLocation = glGetUniformLocation( program, "eye");
	glUniformMatrix4fv(projection, 1, GL_TRUE, _current_Projection);
	glUniformMatrix4fv(Camera_view, 1, GL_TRUE, _currentCamera);
	glUniform4f(eyeLocation,eye.x,eye.y,eye.z,eye.w);
	
	for(std::vector<Model*>::iterator it = models.begin(); it != models.end(); ++it)  
	{
		(*it)->drawTexture(program,_texturesIds[tex_index],_textids[tex_index]);
	}

}
void Scene::enviroment()
{
		
	glEnable(GL_TEXTURE_2D);
	
	GLuint Camera_view = glGetUniformLocation(program,"CameraView");
	GLuint projection = glGetUniformLocation( program, "Projection");
	GLuint eyeLocation = glGetUniformLocation( program, "eye");
	glUniformMatrix4fv(projection, 1, GL_TRUE, _current_Projection);
	glUniformMatrix4fv(Camera_view, 1, GL_TRUE, _currentCamera);
	glUniform4f(eyeLocation,eye.x,eye.y,eye.z,eye.w);
	
	for(std::vector<Model*>::iterator it = models.begin(); it != models.end(); ++it)  
	{
		(*it)->drawEnviroment(program,_texturesIds[6],_texturesIds[tex_index]);
	}

}
void Scene::redraw()
{
	if(_programChanged)
		draw();
	
	/*if(_currentProgram==program_enviroment)
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		background();
		return;
	}
	else*/
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	for(std::vector<Model*>::iterator it = models.begin(); it != models.end(); ++it)  
	{
		(*it)->reDraw(program,_currentProgram);
	}
}

<<<<<<< HEAD
void Scene::background()
{
	vec2 squareVertices[] = {
	vec2( -1.0, -1.0),
    vec2(1.0f, -1.0f),
    vec2(-1.0f,  1.0f),
    vec2(1.0f,  1.0f)
};
	vec2 texCoords[] = {
    vec2( 0.125, 1.0),
    vec2(0.875, 1.0),
    vec2(0.125, 0.0),
    vec2(0.875, 0.0)
};
	
	GLuint background_program = InitShader( "BackgroundVshader.glsl","BackgroundFshader.glsl");
	
	glUseProgram( background_program );
	GLuint buffer[2];
	glGenBuffers(2,buffer);
	glBindBuffer( GL_ARRAY_BUFFER, buffer[0]);
	glBufferData( GL_ARRAY_BUFFER,4* sizeof(vec2), squareVertices, GL_STATIC_DRAW);
	glBindBuffer( GL_ARRAY_BUFFER, buffer[1]);
	glBufferData( GL_ARRAY_BUFFER,4* sizeof(vec2), texCoords, GL_STATIC_DRAW);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[0]);
	GLuint vPosition = glGetAttribLocation( background_program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer( vPosition/*atrib*/, 2/*size*/, GL_FLOAT/*type*/,
		GL_FALSE/*normalized*/, 0/*stride*/, 0/*pointer*/);
	glBindBuffer( GL_ARRAY_BUFFER, buffer[1]);
	GLuint tcoor = glGetAttribLocation( background_program, "tCoor");
	glEnableVertexAttribArray(tcoor);
	glVertexAttribPointer( tcoor/*atrib*/, 2/*size*/, GL_FLOAT/*type*/,
		GL_FALSE/*normalized*/, 0/*stride*/, 0/*pointer*/);
	glActiveTexture(GL_TEXTURE2);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,_texturesIds[6]);
	GLuint tex_loc = glGetUniformLocation(background_program,"texMap"); 
	glUniform1i(tex_loc,2);
	glDrawArrays(GL_QUADS,0,1);
}
void Scene::transformActiveModel(mat4 translation)
{
	_activeModel->setObjectTransform(_activeModel->getObjectTransform() * translation);
}
void Scene::drawSilhoette()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	GLuint program = InitShader("SilhouetteShaderV.glsl", "SilhouetteShaderF.glsl");
	glUseProgram( program );
	GLuint Camera_view = glGetUniformLocation(program,"CameraView");
	GLuint projection = glGetUniformLocation( program, "Projection");
	GLuint eyeLocation = glGetUniformLocation( program, "eye");
	
	glUniformMatrix4fv(projection, 1, GL_TRUE, _current_Projection);
	glUniformMatrix4fv(Camera_view, 1, GL_TRUE, _currentCamera);
	glUniform4f(eyeLocation,eye.x,eye.y,eye.z,eye.w);
	for(std::vector<Model*>::iterator it = models.begin(); it != models.end(); ++it)  
	{
		(*it)->drawSilhoette();
	}
	
}
void Scene::loadTexture()
{
	loadPng ("..\\obj\\cowUV.png",GL_TEXTURE0 ,0);
	loadPng ("..\\obj\\s.png",GL_TEXTURE1 ,1);
	loadPng ("..\\obj\\normalMap.png",GL_TEXTURE2, 2);
	loadPng ("..\\obj\\brick.png",GL_TEXTURE3 ,3);
	loadPng ("..\\obj\\normal.png",GL_TEXTURE4, 4);
	loadMarble(GL_TEXTURE5, 5);
	loadPng ("..\\obj\\wallpaper.png",GL_TEXTURE5, 6);
	loadPng ("..\\obj\\shirt.png",GL_TEXTURE5, 7);
}
void Scene::loadMarble(const int textureUnit, const int textureNumber)
{
	GLubyte* texels = Utils::getInstance().marble();
	_textids[textureNumber]=textureUnit;
	glActiveTexture(textureUnit);
	glGenTextures(1,&_texturesIds[textureNumber]);
	glBindTexture(GL_TEXTURE_2D,_texturesIds[textureNumber]);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,noiseWidth,noiseHeight,0,GL_RGB,GL_UNSIGNED_BYTE,texels);

	glGenerateMipmap(GL_TEXTURE_2D);
=======
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
	lightsUBO << lights;
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

	lightsUBO << lights;
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
	lightsUBO << lights;
	draw();
	cerr << " light direction is " << activeLight->position << "\n";
>>>>>>> refs/heads/final-hw3
}
void Scene::loadPng (const char* fileName, const int textureUnit, const int textureNumber)
{
	PngWrapper pngFile(fileName);
	pngFile.ReadPng();
	int width = pngFile.GetWidth();
	int height = pngFile.GetHeight();
	GLubyte* texels = new GLubyte[width*height*3];
	for (int i=0; i< height; ++i)
	{
		for (int j=0; j< width; j++)
		{
			int value = pngFile.GetValue(j, height -i -1);
			texels[INDEX_PNG(width, i, j ,0)] = GET_R(value);
			texels[INDEX_PNG(width, i, j ,1)] = GET_G(value);
			texels[INDEX_PNG(width, i, j ,2)] = GET_B(value);
		}
	}
	/*_textids[textureNumber]=textureUnit;
	glActiveTexture(textureUnit);*/
	glGenTextures(1,&_texturesIds[textureNumber]);
	glBindTexture(GL_TEXTURE_2D,_texturesIds[textureNumber]);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,texels);

	glGenerateMipmap(GL_TEXTURE_2D);
	
}

void Scene::nextTexture()
{
<<<<<<< HEAD
	tex_index = (++tex_index)%8;
	_programChanged = true;
}
void Scene::setTextureNum(int num)
{
	tex_index=num;
=======
	if(activeLight == NULL) return;
	activeLight->position = rotation * activeLight->position;
	lightsUBO << lights;
	draw();
	cerr << " light location is " << activeLight->position << "\n";
}

void Scene::setFog(vec3 fogColor,GLfloat density)
{
	//if(density)
	//	m_renderer->setFog(fogColor,density);
	//else
	//	m_renderer->clearFog();
	//_addFog = density !=0;
	//draw();
>>>>>>> refs/heads/final-hw3
}