#include "StdAfx.h"
#include "Renderer.h"
#include "CG_skel_w_MFC.h"
#include "InitShader.h"
#include "GL\freeglut.h"
#include <math.h>
#include "Utils.h"
//#include "Line.h"
#include "Camera.h"
#define INDEX(width,x,y,c) ((x+y*width)*3+c)
#define INDEXOF(width,x,y) (x+y*width)
#define LINE(p1,p2,color) Line(*this, p1,p2, color)
#define DLINE(p1,p2,color) LINE(p1,p2,color).draw()
void Renderer::resetZBuffer()
{
	for (int i = 0; i < m_width*m_height; i++)
	{
		m_zbuffer[i]=-10000;
	}
}
Renderer::Renderer() :m_width(MAIN_WIDTH), m_height(MAIN_HEIGHT),drawNormal(false),drawBound(false),lights()
{
	InitOpenGLRendering();
	CreateBuffers(MAIN_WIDTH,MAIN_HEIGHT);
	initMvp();
}
Renderer::Renderer(int width, int height) :m_width(width), m_height(m_height),drawNormal(true),drawBound(true),lights()
{
	InitOpenGLRendering();
	CreateBuffers(width,height);
	initMvp();
}

Renderer::~Renderer(void) { }
void Renderer::initMvp() {
	Mvp = mat4(
		vec4(m_width/2, 0, 0, (m_width-1)/2),
		vec4(0, m_height/2, 0, (m_height-1)/2),
		vec4(0, 0, 1, 0),
		vec4(0, 0, 0, 1));

	std::cerr << "Mvp:\n "<< Mvp << "\n";
	_oTransform = mat4(vec3(1,0,0), vec3(0,1,0), vec3(0,0,1), vec3(0,0,0));
}

void Renderer::CreateBuffers(int width, int height)
{
	m_width=width;
	m_height=height;	
	CreateOpenGLBuffer(); //Do not remove this line.
	m_outBuffer = new float[3*m_width*m_height];
	m_zbuffer = new float[m_width*m_height];

}

void Renderer::SetDemoBuffer()
{
	//vertical line
	for(int i=0; i<m_width; i++)
	{
		m_outBuffer[INDEX(m_width,256,i,0)]=1;	m_outBuffer[INDEX(m_width,256,i,1)]=0;	m_outBuffer[INDEX(m_width,256,i,2)]=0;

	}
	//horizontal line
	for(int i=0; i<m_width; i++)
	{
		m_outBuffer[INDEX(m_width,i,256,0)]=1;	m_outBuffer[INDEX(m_width,i,256,1)]=0;	m_outBuffer[INDEX(m_width,i,256,2)]=1;
	}	
}


float *Renderer::getOutBuffer() {
	return m_outBuffer;
}

int Renderer::getHeight() const{
	return m_height;
}

int Renderer::getWidth() const{
	return m_width;
}

/////////////////////////////////////////////////////
//OpenGL stuff. Don't touch.

void Renderer::InitOpenGLRendering()
{
	int a = glGetError();
	a = glGetError();
	glGenTextures(1, &gScreenTex);
	a = glGetError();
	glGenVertexArrays(1, &gScreenVtc);
	GLuint buffer;
	glBindVertexArray(gScreenVtc);
	glGenBuffers(1, &buffer);
	const GLfloat vtc[]={
		-1, -1,
		1, -1,
		-1, 1,
		-1, 1,
		1, -1,
		1, 1
	};
	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
	glUseProgram( program );
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );

	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0,
		0 );

	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		(GLvoid *) sizeof(vtc) );
	glUniform1i(glGetUniformLocation(program, "texture"), 0 );
	a = glGetError();
}

void Renderer::CreateOpenGLBuffer()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gScreenTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_width, m_height, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, m_width, m_height);
}

void Renderer::SwapBuffers()
{

	int a = glGetError();
	glActiveTexture(GL_TEXTURE0);
	a = glGetError();
	glBindTexture(GL_TEXTURE_2D, gScreenTex);
	a = glGetError();
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGB, GL_FLOAT, m_outBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	a = glGetError();

	glBindVertexArray(gScreenVtc);
	a = glGetError();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	a = glGetError();
	glutSwapBuffers();
	a = glGetError();
}

void Renderer::SetProjection(const mat4& projection)
{
	_projection = projection;
}

void Renderer::SetCameraTransform(const mat4& cTransform) {
	_cTransform = cTransform;
}

void Renderer::SetObjectMatrices(const mat4& oTransform, const mat3& nTransform) {

}

void Renderer::SetObjectMatrices(const mat4& oTransform) {
	_oTransform = oTransform;
	_composition = _cTransform * _oTransform;
}

mat4 Renderer::getProjection()
{
	return _projection;
}

bool Renderer::DrawTriangle( Face face,vec3 color)
{
	vec3 _v1 = face.getVecX();
	vec3 _v2 = face.getVecY();
	vec3 _v3 = face.getVecZ();
	bool flag= true;
	Face newFace =face.transformFace(*this);
	vec3 v1 = calculateMvpPCTransformation(_v1);
	vec3 v2 = calculateMvpPCTransformation(_v2);
	vec3 v3 = calculateMvpPCTransformation(_v3);
	vec3 tmpNormal = newFace.getNormal();			// normal for flat shading
	GLfloat faceDirection=  dot(normalize(activeCamera->getEye()-activeCamera->getAt()),tmpNormal);
	int maxX = (v1.x<v2.x)?(v2.x<v3.x?v3.x:v2.x):(v1.x<v3.x?v3.x:v1.x);
	maxX = maxX < m_width ? maxX : m_width;
	int maxY = (v1.y<v2.y)?(v2.y<v3.y?v3.y:v2.y):(v1.y<v3.y?v3.y:v1.y);
	maxY = maxY < m_height ? maxY : m_height;
	int minX = (v1.x>v2.x)?(v2.x>v3.x?v3.x:v2.x):(v1.x>v3.x?v3.x:v1.x);
	minX = minX > 0 ? minX : 0;
	int minY = (v1.y>v2.y)?(v2.y>v3.y?v3.y:v2.y):(v1.y>v3.y?v3.y:v1.y);
	minY = minY > 0 ? minY : 0;
	if(faceDirection>0)	
	for (int i = minY; i <= maxY; i++)
	{
		for (int j = minX; j <= maxX; j++)
		{
			Face frameFace(v1,v2,v3);
			GLfloat z = Utils::interpolateFace(frameFace,j,i);
			vec3 bCordinated = Utils::getInstance().getBarycentricCoordinates(frameFace,j,i,z);
			if(bCordinated.x >= 0 && bCordinated.x <= 1 && bCordinated.y >= 0 && bCordinated.y <= 1
				&& bCordinated.z >= 0 && bCordinated.z <= 1)
			{
				mat4 tmpCamera = _cTransform;
				mat4 tmpCom = _composition;
				mat4 tmp_projection = _projection;
				SetCameraTransform(mat4(1));
				SetProjection(mat4(1));
				Face transformed = face.transformFace(*this);
				SetCameraTransform(tmpCamera);
				SetProjection(tmp_projection);
				vec3 interpolatedNormal  = bCordinated.y * transformed.getVnX() + bCordinated.z * transformed.getVnY() + 
					bCordinated.x * transformed.getVnZ();
				plot(transformed,frameFace,j,i,color,normalize(interpolatedNormal) );
					
			}
		}
	}
	return flag;
}

bool Renderer::plot(Face worldFace,Face frameFace, int x, int y,vec3 color,vec3 normal) 
{
	bool flag = true;
	vec3 baryCordinate = Utils::getInstance().getBarycentricCoordinates(frameFace,x,y,Utils::interpolateFace(frameFace,x,y));
	GLfloat zcordinate = Utils::interpolateFace(frameFace,x,y);
	vec3 worldCordinated = worldFace.getVecX()* baryCordinate.y+worldFace.getVecY()* baryCordinate.z +worldFace.getVecZ()* baryCordinate.x;
	if(x<m_width && x>0 && y>0 && y<m_height)
	{
		if(m_zbuffer[INDEXOF(m_width,x,y)]<zcordinate)
		{
			vec3 light = getLightFactorForPoint(worldCordinated.x,worldCordinated.y,worldCordinated.z, normal,worldFace);
			color /=255.0;
			m_outBuffer[INDEX(m_width,x,y,2)]= color.x * light.x;
			m_outBuffer[INDEX(m_width,x,y,1)]= color.y * light.y;
			m_outBuffer[INDEX(m_width,x,y,0)]= color.z * light.z;

			m_zbuffer[INDEXOF(m_width,x,y)]=zcordinate;
		}
		else
			flag= false;
	}
	return flag;
}
vec3 Renderer::getLightFactorForPoint(GLfloat x,GLfloat y,GLfloat z,vec3 normal,Face& f)
{
	vec3 point(x,y,z);
	vec3 light = (0.0,0.0,0.0);
	for (vector<Light*>::iterator it = lights.begin();it!= lights.end();++it)
	{
		light += (*it)->getLight(normal,_kAmbiant,_kDiffuze,_kspecular,vec3(x,y,z), _cTransform,f,activeCamera->getEye()-activeCamera->getAt(),_shine);
	}
	return light;
}
vec3 Renderer::calculateMvpPCTransformation(vec4 worldPoint,bool mode) 
{
	vec4 transformed;
	 if(mode)
		 transformed = Mvp * _projection * _cTransform *_oTransform* worldPoint;
	 else
		 transformed =  _projection * _cTransform *_oTransform* worldPoint;
	if(transformed.w)
		return vec3(transformed.x/transformed.w, transformed.y/transformed.w,transformed.z/transformed.w);
	return vec3(transformed.x, transformed.y,transformed.z);
}

vec2 Renderer::calculateTransformation(vec4 relativePoint) { // USE THIS FUNCTION!
	vec4 transformed = Mvp * _projection * _cTransform * _oTransform * relativePoint;
	if(transformed.w)
		return vec2(transformed.x/transformed.w, transformed.y/transformed.w);
	return vec2(transformed.x, transformed.y);
}

void Renderer::drawLineByVectors( vec3 from ,vec3 to,unsigned int color) {

	vec2 vOut1 = calculateTransformation(from);
	vec2 vOut2 = calculateTransformation(to);
	if(vOut1.x<m_width && vOut1.x>0 &&vOut1.y>0 &&vOut1.y<m_height
		&& vOut2.x<m_width && vOut2.x>0 &&vOut2.y>0 &&vOut2.y<m_height)
	DLINE(vOut1,vOut2,color);
}

void Renderer::drawLineByVectors(vec3 from ,vec3 to,bool normal)
{
	std::cerr << "Renderer::drawLineByVectors(vec3 from ,vec3 to,bool normal) is Deprecated\n";
}

void Renderer::drawAxis(mat4 axisMove)
{
	SetObjectMatrices(mat4(1));
	vec4 zeroPoint= Mvp * _cTransform * axisMove * vec4(0,0,0,1);
	vec4 axisX = Mvp * _cTransform * axisMove * vec4(100,0,0,1);
	vec4 axisY = Mvp * _cTransform * axisMove * vec4(0,100,0,1);
	vec4 axisZ = Mvp * _cTransform * axisMove * vec4(0,0,100,1);
	DLINE(vec2(zeroPoint.x,zeroPoint.y)/zeroPoint.w,vec2(axisX.x,axisX.y)/axisX.w,RED);
	DLINE(vec2(zeroPoint.x,zeroPoint.y)/zeroPoint.w,vec2(axisY.x,axisY.y)/axisY.w,GREEN);
	DLINE(vec2(zeroPoint.x,zeroPoint.y)/zeroPoint.w,vec2(axisZ.x,axisZ.y)/axisZ.w,BLUE);	
}

//TODO draw triangle
bool Renderer::isDrawNormal()
{
	return drawNormal;
}
void Renderer::setDrawnormal(bool _drawNormal)
{
	drawNormal= _drawNormal;
}

bool Renderer::isDrawBound()
{
	return drawBound;
}
void Renderer::setDrawBound(bool isDrawBound)
{
	drawBound= isDrawBound;
}
void Renderer::ClearColorBuffer()
{
	for(int x = 0 ;x<m_width ;x++)
		for(int y = 0 ; y < m_height ;y++)
		{	
			m_outBuffer[INDEX(m_width,x,y,2)]=  BLACK			& 0x0000ff;
			m_outBuffer[INDEX(m_width,x,y,1)]= (BLACK >> 8)		& 0x0000ff;
			m_outBuffer[INDEX(m_width,x,y,0)]= (BLACK >> 16)	& 0x0000ff;
		}
	//SwapBuffers();
}

Renderer::Line::Line(vec2 p1, vec2 p2, int color): _p1(p1), _p2(p2), horizontal(false), vertical(false), m_outBuffer(0), m_color(color) {
	//calculateSlope(_p1, _p2, true);
	calculateSlope();
}

Renderer::Line::Line(Renderer &renderer, vec2 p1, vec2 p2, int color): _p1(p1), _p2(p2), horizontal(false), vertical(false), m_outBuffer(0), m_color(color) {
	m_outBuffer = renderer.getOutBuffer();
	m_width = renderer.getWidth();
	m_height = renderer.getHeight();
	calculateSlope();
}

Renderer::Line::~Line(void)
{
}

void Renderer::Line::draw() {
	draw(m_outBuffer);
}

void Renderer::Line::horizontalLine(float *outBuffer) {
	if(_p1.x>_p2.x)
		swap(_p1, _p2);

	GLfloat x = _p1.x;
	GLfloat y = _p1.y;
	do {
		plot(x,y);
	}while(++x<=_p2.x);
}

void Renderer::Line::verticalLine(float *outBuffer) {
	if(_p1.y>_p2.y)
		swap(_p1, _p2);

	GLfloat x = _p1.x;
	GLfloat y = _p1.y;
	do {
		plot(x,y);
	}while(++y<=_p2.y);
}

void Renderer::Line::draw(float *outBuffer) {
	if(vertical) { // Draw a vertical line
		verticalLine(m_outBuffer);
		return;
	}
	if(horizontal) { // Draw a horizontal line
		horizontalLine(m_outBuffer);
		return;
	}
	drawLine();
}

void Renderer::Line::calculateSlope() {
	// Check if line is vertical
	if(_p1.x>_p2.x)		// This one is global to the rest as long as |slope|<1
		swap(_p1, _p2);

	GLfloat dx = _p1.x - _p2.x;
	inverseSteps = false;

	if (abs(dx)<delta_threshold) {
		vertical = true;
		slope = INFINITE;
		if(_p1.y>_p2.y)
			swap(_p1, _p2);
		return;
	}
	// Check if line is horizontal
	GLfloat dy = _p1.y - _p2.y;
	if (abs(dy)<delta_threshold) {
		horizontal = true;
		slope = 0;
		return;
	}
	slope = dy/dx;

	if(abs(slope)>1) {// Inverse the Bernsomething algo
		if(_p1.y > _p2.y)
			swap(_p1, _p2);
		inverseSteps = true;
	}

}

void Renderer::Line::drawLine()
{
	vec2 p1(_p1);
	vec2 p2(_p2);

	if(inverseSteps) { // Swap x and y
		swap(p1.x, p1.y);
		swap(p2.x, p2.y);
	}

	int dx, dy, d, de, dne;
	int x, y, x2, y2;

	dx = p2.x - p1.x; 
	dy = p2.y - p1.y; 
	
	x = p1.x; y = p1.y; 
	x2 = p2.x; y2 = p2.y;
	
	if(slope>0) {
		d = 2*dy - dx;	
		dne = 2*(dy-dx);
	} else {
		d = 2*dy + dx;
		dne = 2*(dy+dx);
	}

	de = 2*dy;

	if(inverseSteps)
		plot(y,x);
	else
		plot(x, y);
	
	while(x<x2) 
	{
		if(d<0)
		{
			if(slope>0)
				d = d + de;
			else
				d = d - de;
			x += 1;
		} else {
			if(slope>0)
				d = d + dne;
			else
				d = d - dne;
			x += 1;
			if(slope<0)
				y -= 1;
			else
				y += 1;
		}
		if(inverseSteps)
			plot(y,x);
		else
			plot(x, y);
	}
}
