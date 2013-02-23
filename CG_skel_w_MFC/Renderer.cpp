#include "StdAfx.h"
//#include "Renderer.h"
//#include "CG_skel_w_MFC.h"
//#include "InitShader.h"
//#include "GL\freeglut.h"
//#include <math.h>
//#include "Utils.h"
////#include "Line.h"
//#include "Camera.h"
//#define LINE(p1,p2,color) Line(*this, p1,p2, color)
//#define DLINE(p1,p2,color) LINE(p1,p2,color).draw()
//void Renderer::resetZBuffer()
//{
//	GLfloat *zbuffer = getZBuffer();
//	
//	int width = getWidth();
//	int height = getHeight();
//	SetObjectMatrices(mat4(1));
//	vec4 eye_at = normalize( activeCamera->getEye()- activeCamera->getAt());
//	
//	for (int j = 0; j < height; j++)
//	{
//		for(int i = 0 ; i < width ; i++)
//		{
//			zbuffer[INDEXOF(width,i,j)]=-_zfar;
//		}
//	}
////	cout <<"eye "<< eye4 << "\n";
//}
//Renderer::Renderer() : antiAliasing(false), m_width(MAIN_WIDTH), m_height(MAIN_HEIGHT),drawNormal(false),drawBound(false),lights()
//{
//	InitOpenGLRendering();
//	CreateBuffers(MAIN_WIDTH,MAIN_HEIGHT);
//	initMvp();
//	fog = NULL;
//}
//Renderer::Renderer(int width, int height) :antiAliasing(false),m_width(width), m_height(m_height),drawNormal(true),drawBound(true),lights()
//{
//	InitOpenGLRendering();
//	CreateBuffers(width,height);
//	initMvp();
//	fog = NULL;
//}
//
//Renderer::~Renderer(void) { }
//
//void Renderer::initMvp() {
//	int width = m_width;
//	int height = m_height;
//
//	if(antiAliasing) {
//		width *= 2;
//		height *= 2;
//	}
//
//	Mvp = mat4(
//		vec4(width/2, 0, 0, (width-1)/2),
//		vec4(0, height/2, 0, (height-1)/2),
//		vec4(0, 0, 1, 0),
//		vec4(0, 0, 0, 1));
//
//	std::cerr << "Mvp:\n "<< Mvp << "\n";
//	_oTransform = mat4(vec3(1,0,0), vec3(0,1,0), vec3(0,0,1), vec3(0,0,0));
//}
//
//void Renderer::CreateBuffers(int width, int height)
//{
//	m_width=width;
//	m_height=height;	
//	CreateOpenGLBuffer(); //Do not remove this line.
//	m_outBuffer = new float[3*m_width*m_height];
//	
//	// Screw efficiency
//	m_aliased_outBuffer = new float[3*m_width*m_height*4];
//	m_aliased_zbuffer = new float[3*m_width*m_height*4];
//	
//	m_zbuffer = new float[m_width*m_height];
//	
//}
//
//void Renderer::SetDemoBuffer()
//{
//
//	////vertical line
//	//for(int i=0; i<m_width; i++)
//	//{
//	//	m_outBuffer[INDEX(m_width,256,i,0)]=1;	m_outBuffer[INDEX(m_width,256,i,1)]=0;	m_outBuffer[INDEX(m_width,256,i,2)]=0;
//
//	//}
//	////horizontal line
//	//for(int i=0; i<m_width; i++)
//	//{
//	//	m_outBuffer[INDEX(m_width,i,256,0)]=1;	m_outBuffer[INDEX(m_width,i,256,1)]=0;	m_outBuffer[INDEX(m_width,i,256,2)]=1;
//	//}	
//	cerr << "Legacy - will be removed\n";
//}
//
//
//float *Renderer::getOutBuffer() {
//	if(antiAliasing)
//		return m_aliased_outBuffer;
//	
//	return m_outBuffer;
//}
//
//float *Renderer::getZBuffer() {
//	if(antiAliasing)
//		return m_aliased_zbuffer;
//
//	return m_zbuffer;
//}
//
//int Renderer::getHeight() const{
//	if(antiAliasing)
//		return m_height*2;
//	
//	return m_height;
//}
//
//int Renderer::getWidth() const{
//	if(antiAliasing)
//		return m_width*2;
//
//	return m_width;
//}
//
///////////////////////////////////////////////////////
////OpenGL stuff. Don't touch.
//
//void Renderer::InitOpenGLRendering()
//{
//	int a = glGetError();
//	a = glGetError();
//	glGenTextures(1, &gScreenTex);
//	a = glGetError();
//	glGenVertexArrays(1, &gScreenVtc);
//	GLuint buffer;
//	glBindVertexArray(gScreenVtc);
//	glGenBuffers(1, &buffer);
//	const GLfloat vtc[]={
//		-1, -1,
//		1, -1,
//		-1, 1,
//		-1, 1,
//		1, -1,
//		1, 1
//	};
//	const GLfloat tex[]={
//		0,0,
//		1,0,
//		0,1,
//		0,1,
//		1,0,
//		1,1};
//	glBindBuffer(GL_ARRAY_BUFFER, buffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);
//	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);
//	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);
//
//	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
//	glUseProgram( program );
//	GLint  vPosition = glGetAttribLocation( program, "vPosition" );
//
//	glEnableVertexAttribArray( vPosition );
//	glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0,
//		0 );
//
//	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
//	glEnableVertexAttribArray( vTexCoord );
//	glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
//		(GLvoid *) sizeof(vtc) );
//	glUniform1i(glGetUniformLocation(program, "texture"), 0 );
//	a = glGetError();
//}
//
//void Renderer::CreateOpenGLBuffer()
//{
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, gScreenTex);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_width, m_height, 0, GL_RGB, GL_FLOAT, NULL);
//	glViewport(0, 0, m_width, m_height);
//}
//
//GLfloat Renderer::sumQuad(int x, int y, int color) {
//		float k = 0;
//		x *= 2; y *= 2;
//		k += m_aliased_outBuffer[INDEX(m_width*2, x, y, color)];
//		k += m_aliased_outBuffer[INDEX(m_width*2, x+1, y, color)];
//		k += m_aliased_outBuffer[INDEX(m_width*2, x, y+1, color)];
//		k += m_aliased_outBuffer[INDEX(m_width*2, x+1, y+1, color)];
//		k /= 4;
//		return k;
//}
//
//void Renderer::doAntialiasing() {
//	for(int x=0; x<m_width; ++x) {
//		for(int y=0; y<m_height; ++y) {
//			m_outBuffer[INDEX(m_width, x, y, 2)] = sumQuad(x,y, 2);
//			m_outBuffer[INDEX(m_width, x, y, 1)] = sumQuad(x,y, 1);
//			m_outBuffer[INDEX(m_width, x, y, 0)] = sumQuad(x,y, 0);
//		}
//	}
//}
//
//void Renderer::SwapBuffers()
//{
//	// TODO: Anti aliasing here
//	if(antiAliasing) 
//		doAntialiasing();
//
//	int a = glGetError();
//	glActiveTexture(GL_TEXTURE0);
//	a = glGetError();
//	glBindTexture(GL_TEXTURE_2D, gScreenTex);
//	a = glGetError();
//	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGB, GL_FLOAT, m_outBuffer);
//	glGenerateMipmap(GL_TEXTURE_2D);
//	a = glGetError();
//
//	glBindVertexArray(gScreenVtc);
//	a = glGetError();
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//	a = glGetError();
//	glutSwapBuffers();
//	a = glGetError();
//}
//
//void Renderer::SetProjection(const mat4& projection)
//{
//	_projection = projection;
//}
//
//void Renderer::SetCameraTransform(const mat4& cTransform) {
//	_cTransform = cTransform;
//}
//
//void Renderer::SetObjectMatrices(const mat4& oTransform, const mat3& nTransform) {
//
//}
//
//void Renderer::SetObjectMatrices(const mat4& oTransform) {
//	_oTransform = oTransform;
//	_composition = _cTransform * _oTransform;
//}
//
//mat4 Renderer::getProjection()
//{
//	return _projection;
//}
//
//bool Renderer::DrawTriangle( Face face,vec3 color,bool flat)
//{
//	throw exception();
///*
//	int width = getWidth(); int height = getHeight();
//	vec3 _v1 = face.getVecX();
//	vec3 _v2 = face.getVecY();
//	vec3 _v3 = face.getVecZ();
//	bool flag= true;
//	vec4 objectV1_4 = _oTransform * _v1;
//	vec4 objectV2_4 = _oTransform * _v2;
//	vec4 objectV3_4 = _oTransform * _v3;
//	vec3 objectv1(objectV1_4.x,objectV1_4.y,objectV1_4.z);
//	vec3 objectv2(objectV2_4.x,objectV2_4.y,objectV2_4.z);
//	vec3 objectv3(objectV3_4.x,objectV3_4.y,objectV3_4.z);
//	vec3 midpoint = (objectv1 + objectv2 + objectv3) /3;
//	vec3 v1 = calculateMvpPCTransformation(_v1);
//	vec3 v2 = calculateMvpPCTransformation(_v2);
//	vec3 v3 = calculateMvpPCTransformation(_v3);
//	vec3 tmpNormal = normalize(cross(objectv2-objectv1,objectv3-objectv1));
//	GLfloat faceDirection=  dot(normalize(activeCamera->getEye()-midpoint),tmpNormal);
//	int maxX = (v1.x<v2.x)?(v2.x<v3.x?v3.x:v2.x):(v1.x<v3.x?v3.x:v1.x);
//	maxX = maxX < width ? maxX : width;
//	int maxY = (v1.y<v2.y)?(v2.y<v3.y?v3.y:v2.y):(v1.y<v3.y?v3.y:v1.y);
//	maxY = maxY < height ? maxY : height;
//	int minX = (v1.x>v2.x)?(v2.x>v3.x?v3.x:v2.x):(v1.x>v3.x?v3.x:v1.x);
//	minX = minX > 0 ? minX : 0;
//	int minY = (v1.y>v2.y)?(v2.y>v3.y?v3.y:v2.y):(v1.y>v3.y?v3.y:v1.y);
//	minY = minY > 0 ? minY : 0;
//	if( minX > width || minY > height || maxX < 0 || maxY <0)
//		return true;
//
//	if(faceDirection>0)	
//	for (int i = minY; i <= maxY; i++)
//	{
//		for (int j = minX; j <= maxX; j++)
//		{
//			Face frameFace(v1,v2,v3);
//			GLfloat z = Utils::interpolateFace(frameFace,j,i);
//			vec3 bCordinated = Utils::getInstance().getBarycentricCoordinates(frameFace,j,i,z);
//			if(bCordinated.x >= 0 && bCordinated.x <= 1 && bCordinated.y >= 0 && bCordinated.y <= 1
//				&& bCordinated.z >= 0 && bCordinated.z <= 1)
//			{
//				mat4 tmpCamera = _cTransform;
//				mat4 tmpCom = _composition;
//				mat4 tmp_projection = _projection;
//				SetCameraTransform(mat4(1));
//				SetProjection(mat4(1));
//				Face transformed = face.transformFace(*this);
//				SetCameraTransform(tmpCamera);
//				SetProjection(tmp_projection);
//				vec3 interpolatedNormal  = bCordinated.y * transformed.getVnX() + bCordinated.z * transformed.getVnY() + 
//					bCordinated.x * transformed.getVnZ();
//				plot(transformed,frameFace,j,i,color,flat?transformed.getNormal():normalize(interpolatedNormal),bCordinated,z );
//			}
//		}
//	}
//	return flag;
//	*/
//}
//
//void Renderer::DrawTriangleFrech(Face face,vec3 color)
//{
//	int width = getWidth(); int height = getHeight();
//	vec4 objectV1_4 = _oTransform * face.getVecX();
//	vec4 objectV2_4 = _oTransform * face.getVecY();
//	vec4 objectV3_4 = _oTransform * face.getVecZ();
//	vec3 objectv1(objectV1_4.x,objectV1_4.y,objectV1_4.z);
//	vec3 objectv2(objectV2_4.x,objectV2_4.y,objectV2_4.z);
//	vec3 objectv3(objectV3_4.x,objectV3_4.y,objectV3_4.z);
//	vec3 tmpNormal = normalize(cross(objectv2-objectv1,objectv3-objectv1));
//	GLfloat faceDirection=  dot((activeCamera->getEye()-(objectv1 + objectv2 + objectv3)/3),tmpNormal);
//	if(faceDirection < 0) return;
//	mat4 tmpCamera = _cTransform;
//	mat4 tmpCom = _composition;
//	mat4 tmp_projection = _projection;
//	SetCameraTransform(mat4(1));
//	SetProjection(mat4(1));
//	Face transformed = face.transformFace(*this);
//	SetCameraTransform(tmpCamera);
//	SetProjection(tmp_projection);
//	vec4 P1 = transformed.getVecX();
//	vec4 P2 = transformed.getVecY();
//	vec4 P3 = transformed.getVecZ();
//
//	vec3 lightP1 = getLightFactorForPoint(P1.x,P1.y,P1.z, transformed.getVnX(),transformed);
//	vec3 lightP2 = getLightFactorForPoint(P2.x,P2.y,P2.z, transformed.getVnY(),transformed);
//	vec3 lightP3 = getLightFactorForPoint(P3.x,P3.y,P3.z, transformed.getVnZ(),transformed);
//
//	Face mvpFace = face.transformFace(*this,true); 
//	Face frameFace = face.transformFace(*this);
//	vec3 v1 = mvpFace.getVecX();
//	vec3 v2 = mvpFace.getVecY();
//	vec3 v3 = mvpFace.getVecZ();
//
//	int maxX = (v1.x<v2.x)?(v2.x<v3.x?v3.x:v2.x):(v1.x<v3.x?v3.x:v1.x);
//	maxX = maxX < width ? maxX : width;
//	int maxY = (v1.y<v2.y)?(v2.y<v3.y?v3.y:v2.y):(v1.y<v3.y?v3.y:v1.y);
//	maxY = maxY < height ? maxY : height;
//	int minX = (v1.x>v2.x)?(v2.x>v3.x?v3.x:v2.x):(v1.x>v3.x?v3.x:v1.x);
//	minX = minX > 0 ? minX : 0;
//	int minY = (v1.y>v2.y)?(v2.y>v3.y?v3.y:v2.y):(v1.y>v3.y?v3.y:v1.y);
//	minY = minY > 0 ? minY : 0;
//	if( minX > width || minY > height || maxX < 0 || maxY <0)
//		return;
//
//	for (int i = minY; i <= maxY; i++)
//	{
//		for (int j = minX; j <= maxX; j++)
//		{
//			GLfloat z = Utils::interpolateFace(mvpFace,j,i);
//			vec3 bCordinated = Utils::getInstance().getBarycentricCoordinates(mvpFace,j,i,z);
//			if(bCordinated.x >= 0 && bCordinated.x <= 1 && bCordinated.y >= 0 && bCordinated.y <= 1
//				&& bCordinated.z >= 0 && bCordinated.z <= 1)
//			{
//				mat4 tmpProjection = _projection;
//				mat4 tmpOtransform = _oTransform;
//				SetObjectMatrices(mat4(1));
//				SetProjection(mat4(1));
//				Face f = transformed.transformFace(*this);
//				vec3 faceCor = f.getVecX()* bCordinated.y+f.getVecY()* bCordinated.z +f.getVecZ()* bCordinated.x;
//				SetProjection(tmpProjection);
//				SetObjectMatrices(tmpOtransform);
//				vec3 colors = bCordinated.y * lightP1 + bCordinated.z * lightP2 + 
//					bCordinated.x * lightP3;
//				vec3 worldCordinate = bCordinated.y * transformed.getVecX() + bCordinated.z * transformed.getVecY() 
//					+ bCordinated.x*transformed.getVecZ();
//				
//				putPixel(j,i,faceCor.z, colors);
//			}
//		}
//	}
//
//}
//
//
//void Renderer::putPixel(int x, int y, GLfloat zIndex, vec3 colors) {
//	int width = getWidth();	
//	int height = getHeight();
//
//	if(x>=width || x<0 || y>=height || y < 0) return;
//	GLfloat *zbuff = getZBuffer();
//	if(zbuff[INDEXOF(width, x, y)]>zIndex) return;
//	GLfloat *out = getOutBuffer();
//
//	zbuff[INDEXOF(width, x, y)] = zIndex;
//
//	out[INDEX(width,x,y,2)] = colors.x;
//	out[INDEX(width,x,y,1)] = colors.y;
//	out[INDEX(width,x,y,0)] = colors.z;
//}
//
//
//void Renderer::displayPixel(int x, int y) {
//	cerr << "("<<x << "," << y<<")";
//	if(antiAliasing) {
//		vec3 quads(sumQuad(x,y,2), sumQuad(x,y,1),sumQuad(x,y,0));
//		cerr << "sumQuad: " << quads << "\n";
//	}
//	vec3 screenArray(m_outBuffer[INDEX(m_width,x,y,2)],m_outBuffer[INDEX(m_width,x,y,1)], m_outBuffer[INDEX(m_width,x,y,0)]);
//	cerr << "("<<x << "," << y<<")";
//	cerr << "Display: " << screenArray << "\n";
//	GLfloat *out = getOutBuffer();
//
//	if(antiAliasing) {
//		m_aliased_outBuffer[INDEX(2*m_width,x, y,2)]= 1;
//		m_aliased_outBuffer[INDEX(2*m_width, x+1, y,2)]= 1;
//		m_aliased_outBuffer[INDEX(2*m_width,x, y+1,2)]= 1;
//		m_aliased_outBuffer[INDEX(2*m_width,x+1, y+1,2)]= 1;
//	} else {
//		int width = m_width;
//		int corsair_size = 20;
//		int half_corsair = corsair_size/2;
//		for(int i=0;i<corsair_size;++i) { // Draw an X
//			m_outBuffer[INDEX(m_width, x+i, y, 2)] = 1;
////			m_outBuffer[INDEX(width,x, (m_height-y)-half_corsair+i , 2)] = 1;
////			m_outBuffer[INDEX(width,x-half_corsair+i,(m_height-y) , 2)] = 1;
//		}
//	}
//	SwapBuffers();
//}
//
//bool Renderer::plot(Face worldFace,Face frameFace, int x, int y,vec3 color,vec3 normal,vec3 baryCordinate,GLfloat zcordinate,GLfloat g) 
//{
//
//	bool flag = true;
//	int width = getWidth();
//	int height = getHeight();
//	vec3 worldCordinated = worldFace.getVecX() * baryCordinate.y + worldFace.getVecY() * baryCordinate.z + worldFace.getVecZ() * baryCordinate.x;
//	if(x<width && x>0 && y>0 && y<height)
//	{
//		GLfloat *zbuffer = getZBuffer();
//		GLfloat *outBuffer = getOutBuffer();
//		mat4 tmpProjection = _projection;
//		mat4 tmpOtransform = _oTransform;
//		SetObjectMatrices(mat4(1));
//		SetProjection(mat4(1));
//		Face f = worldFace.transformFace(*this,true);
//		vec3 faceCor = f.getVecX() * baryCordinate.y + f.getVecY() * baryCordinate.z + f.getVecZ() * baryCordinate.x;
//		SetProjection(tmpProjection);
//		SetObjectMatrices(tmpOtransform);
//		if(x<width && x>0 && y>0 && y<height)
//		if(zbuffer[INDEXOF(width,x,y)]<faceCor.z&& faceCor.z < _znear)
//		{
//			color/=255;
//			vec3 light = getLightFactorForPoint(worldCordinated.x,worldCordinated.y,worldCordinated.z, normal,worldFace);
//			if(_cartoonize)
//			{
//				if(light.x >0.5) light.x =1;
//				else light.x =0.5;
//				if(light.y >0.5) light.y =1;
//				else light.y =0.5;
//				if(light.z >0.5) light.z =1;
//				else light.z =0.5;
//			}
//			vec3 cEye = (color * light);
//			
//			int numOfColors = 256/_color;
//			cEye *= 255;
//			cEye = vec3((int)cEye.x/numOfColors,(int)cEye.y/numOfColors,(int)cEye.z/numOfColors);
//			cEye *= numOfColors;
//			cEye /=255;
//			if(_cartoonize)
//			{
//				vec4 eye4 = activeCamera->getEye()-activeCamera->getAt();
//				vec3 eye(eye4.x,eye4.y,eye4.z);
//				if( dot(eye,normal)<=0.2)
//					cEye =vec3(0);
//			}
//			
//			putPixel(x,y, faceCor.z, cEye);
//		}
//		else
//			flag= false;
//	}
//	return flag;
//}
//vec3 Renderer::getLightFactorForPoint(GLfloat x,GLfloat y,GLfloat z,vec3& normal,Face& f)
//{
//	vec3 point(x,y,z);
//	vec3 light = (0.0,0.0,0.0);
//	for (vector<Light*>::iterator it = lights.begin();it!= lights.end();++it)
//	{
//		light += (*it)->getLight(normal,_kAmbiant,_kDiffuze,_kspecular,vec3(x,y,z), _cTransform,f,activeCamera->getEye()-activeCamera->getAt(),_shine);
//	}
//	return light;
//}
//vec3 Renderer::calculateMvpPCTransformation(vec4 worldPoint,bool mode) 
//{
//	vec4 transformed;
//	 if(mode)
//		 transformed = Mvp * _projection * _cTransform *_oTransform* worldPoint;
//	 else
//		 transformed =  _projection * _cTransform *_oTransform* worldPoint;
//	if(transformed.w)
//		return vec3(transformed.x/transformed.w, transformed.y/transformed.w,transformed.z/transformed.w);
//	return vec3(transformed.x, transformed.y,transformed.z);
//}
//
//vec2 Renderer::calculateTransformation(vec4 relativePoint) { // USE THIS FUNCTION!
//	vec4 transformed = Mvp * _projection * _cTransform * _oTransform * relativePoint;
//	if(transformed.w)
//		return vec2(transformed.x/transformed.w, transformed.y/transformed.w);
//	return vec2(transformed.x, transformed.y);
//}
//void Renderer::addFog()
//{
//	int width = getWidth();	
//	int height = getHeight();
//	GLfloat *zbuff = getZBuffer();
//	vec3 fogcolor = fog->getFogColor();
//	GLfloat *out = getOutBuffer();
//	for(int j = 0 ; j < height ; j++)
//	{
//		for(int i = 0 ; i < width ; i++)
//		{
//			GLfloat z = zbuff[INDEXOF(width,i,j)];
//			GLfloat t = (z +_zfar)/Zdistance;
//			vec3 pixel(out[INDEX(width,i,j,2)] , out[INDEX(width,i,j,1)] , out[INDEX(width,i,j,0)]);
//			vec3 output = (t) * pixel + (1-t)* fogcolor;
//			putPixel(i,j,zbuff[INDEXOF(width,i,j)],output);
//		}
//
//	}
//}
//void Renderer::drawLineByVectors( vec3 from ,vec3 to,unsigned int color) {
//
//	vec2 vOut1 = calculateTransformation(from);
//	vec2 vOut2 = calculateTransformation(to);
//	int width = getWidth(); int height = getHeight();
//	if(vOut1.x<width && vOut1.x>0 &&vOut1.y>0 &&vOut1.y<height
//		&& vOut2.x<width && vOut2.x>0 &&vOut2.y>0 &&vOut2.y<height)
//	DLINE(vOut1,vOut2,color);
//}
//
//void Renderer::drawLineByVectors(vec3 from ,vec3 to,bool normal)
//{
//	std::cerr << "Renderer::drawLineByVectors(vec3 from ,vec3 to,bool normal) is Deprecated\n";
//}
//
//void Renderer::drawAxis(mat4 axisMove)
//{
//	SetObjectMatrices(mat4(1));
//	vec4 zeroPoint= Mvp * _cTransform * axisMove * vec4(0,0,0,1);
//	vec4 axisX = Mvp * _cTransform * axisMove * vec4(100,0,0,1);
//	vec4 axisY = Mvp * _cTransform * axisMove * vec4(0,100,0,1);
//	vec4 axisZ = Mvp * _cTransform * axisMove * vec4(0,0,100,1);
//	DLINE(vec2(zeroPoint.x,zeroPoint.y)/zeroPoint.w,vec2(axisX.x,axisX.y)/axisX.w,RED);
//	DLINE(vec2(zeroPoint.x,zeroPoint.y)/zeroPoint.w,vec2(axisY.x,axisY.y)/axisY.w,GREEN);
//	DLINE(vec2(zeroPoint.x,zeroPoint.y)/zeroPoint.w,vec2(axisZ.x,axisZ.y)/axisZ.w,BLUE);	
//}
//
////TODO draw triangle
//bool Renderer::isDrawNormal()
//{
//	return drawNormal;
//}
//void Renderer::setDrawnormal(bool _drawNormal)
//{
//	drawNormal= _drawNormal;
//}
//
//bool Renderer::isDrawBound()
//{
//	return drawBound;
//}
//void Renderer::setDrawBound(bool isDrawBound)
//{
//	drawBound= isDrawBound;
//}
//void Renderer::ClearColorBuffer()
//{
//	//int width = getWidth(); int height = getHeight();
//	for(int x = 0 ;x<m_width ;++x)
//		for(int y = 0 ; y < m_height ;++y)
//		{	
//			m_outBuffer[INDEX(m_width,x,y,2)]= ((GLfloat)(WHITE			& 0x0000ff))/255.0;
//			m_outBuffer[INDEX(m_width,x,y,1)]= ((GLfloat)((WHITE >> 8)		& 0x0000ff))/255.0;
//			m_outBuffer[INDEX(m_width,x,y,0)]= ((GLfloat)((WHITE >> 16)	& 0x0000ff))/255.0;
//		}
//
//	if(antiAliasing) {
//		GLfloat *outBuffer = getOutBuffer();
//		int width = getWidth();
//		int height = getHeight();
//		for(int x = 0 ;x<width ;++x)
//			for(int y = 0 ; y < height ;++y)
//			{	
//				outBuffer[INDEX(width,x,y,2)]= ((GLfloat)(WHITE			& 0x0000ff))/255.0;
//				outBuffer[INDEX(width,x,y,1)]= ((GLfloat)((WHITE >> 8)		& 0x0000ff))/255.0;
//				outBuffer[INDEX(width,x,y,0)]= ((GLfloat)((WHITE >> 16)	& 0x0000ff))/255.0;		
//			}		
//	}
//	//SwapBuffers();
//}
//void Renderer::setFog(vec3 fogColor,GLfloat density)
//{
//	fog= new Fog(fogColor,density);
//}
////Renderer::Line::Line(vec2 p1, vec2 p2, int color): _p1(p1), _p2(p2), horizontal(false), vertical(false), m_outBuffer(0), m_color(color) {
////	//calculateSlope(_p1, _p2, true);
////	calculateSlope();
////}
//
//Renderer::Line::Line(Renderer &renderer, vec2 p1, vec2 p2, int color): _p1(p1), _p2(p2), horizontal(false), vertical(false), m_outBuffer(0), m_color(color) ,m_renderer(renderer)
//{
//	m_outBuffer = renderer.getOutBuffer();
//	m_width = renderer.getWidth();
//	m_height = renderer.getHeight();
//	calculateSlope();
//}
//
//Renderer::Line::~Line(void)
//{
//}
//
//void Renderer::Line::draw() {
//	draw(m_outBuffer);
//}
//
//void Renderer::Line::horizontalLine(float *outBuffer) {
//	if(_p1.x>_p2.x)
//		swap(_p1, _p2);
//
//	GLfloat x = _p1.x;
//	GLfloat y = _p1.y;
//	do {
//		plot(x,y);
//	}while(++x<=_p2.x);
//}
//
//void Renderer::Line::verticalLine(float *outBuffer) {
//	if(_p1.y>_p2.y)
//		swap(_p1, _p2);
//
//	GLfloat x = _p1.x;
//	GLfloat y = _p1.y;
//	do {
//		plot(x,y);
//	}while(++y<=_p2.y);
//}
//
//void Renderer::Line::draw(float *outBuffer) {
//	if(vertical) { // Draw a vertical line
//		verticalLine(m_outBuffer);
//		return;
//	}
//	if(horizontal) { // Draw a horizontal line
//		horizontalLine(m_outBuffer);
//		return;
//	}
//	drawLine();
//}
//
//void Renderer::Line::calculateSlope() {
//	// Check if line is vertical
//	if(_p1.x>_p2.x)		// This one is global to the rest as long as |slope|<1
//		swap(_p1, _p2);
//
//	GLfloat dx = _p1.x - _p2.x;
//	inverseSteps = false;
//
//	if (abs(dx)<delta_threshold) {
//		vertical = true;
//		slope = INFINITE;
//		if(_p1.y>_p2.y)
//			swap(_p1, _p2);
//		return;
//	}
//	// Check if line is horizontal
//	GLfloat dy = _p1.y - _p2.y;
//	if (abs(dy)<delta_threshold) {
//		horizontal = true;
//		slope = 0;
//		return;
//	}
//	slope = dy/dx;
//
//	if(abs(slope)>1) {// Inverse the Bernsomething algo
//		if(_p1.y > _p2.y)
//			swap(_p1, _p2);
//		inverseSteps = true;
//	}
//
//}
//
//void Renderer::Line::drawLine()
//{
//	vec2 p1(_p1);
//	vec2 p2(_p2);
//
//	if(inverseSteps) { // Swap x and y
//		swap(p1.x, p1.y);
//		swap(p2.x, p2.y);
//	}
//
//	int dx, dy, d, de, dne;
//	int x, y, x2, y2;
//
//	dx = p2.x - p1.x; 
//	dy = p2.y - p1.y; 
//	
//	x = p1.x; y = p1.y; 
//	x2 = p2.x; y2 = p2.y;
//	
//	if(slope>0) {
//		d = 2*dy - dx;	
//		dne = 2*(dy-dx);
//	} else {
//		d = 2*dy + dx;
//		dne = 2*(dy+dx);
//	}
//
//	de = 2*dy;
//
//	if(inverseSteps)
//		plot(y,x);
//	else
//		plot(x, y);
//	
//	while(x<x2) 
//	{
//		if(d<0)
//		{
//			if(slope>0)
//				d = d + de;
//			else
//				d = d - de;
//			x += 1;
//		} else {
//			if(slope>0)
//				d = d + dne;
//			else
//				d = d - dne;
//			x += 1;
//			if(slope<0)
//				y -= 1;
//			else
//				y += 1;
//		}
//		if(inverseSteps)
//			plot(y,x);
//		else
//			plot(x, y);
//	}
//}
