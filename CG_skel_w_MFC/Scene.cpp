#include "stdafx.h"
#include "Scene.h"
#include "MeshModel.h"
#include <string>
#include "Utils.h"

//#include "ModelData.h"
#include "InitShader.h"
using namespace std;
#define INDEX_PNG(width,x,y,c) (((y)+(x)*(width))*3+(c))

void Scene::loadOBJModel(MeshModel* model)
{
	_activeModel = model;
	models.push_back(model);
	_programChanged=true;
	redraw();
}
void Scene::LookAt( const vec4& eye, const vec4& at, const vec4& up )
{
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
}
void Scene::setProgramType(programType prog)
{
	_programChanged = true;
	_currentProgram = prog;
	redraw();
}
void Scene::initDefaultCamera() 
{
}
void Scene::initDefaultLight()
{
}
Scene::Scene():models(),_currentProgram(program_Phong),_programChanged(true)
{
	loadTexture();
	tex_index =0;
}
void Scene::setFrustum(float left,float right,float top,float bottom,float zNear,float zFar)
{
	_zfar=zFar;
	_znear = zNear;
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
	tex_index = (++tex_index)%8;
	_programChanged = true;
}
void Scene::setTextureNum(int num)
{
	tex_index=num;
}