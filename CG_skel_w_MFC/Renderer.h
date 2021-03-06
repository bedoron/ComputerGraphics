#pragma once
//#include <vector>
//#include "CG_skel_w_MFC.h"
//#include "mat.h"
//#include "GL/glew.h"
//#include "Face.h"
//#include "Light.h"
//#include "Fog.h"
//class Camera;
//
#define BLACK		0x000000
#define RED			0xff0000
#define GREEN		0x00ff00
#define BLUE		0x0000ff
#define MAGENTA		(RED | BLUE)
#define YELLOW		(RED | GREEN)
#define AQUA		(GREEN | BLUE)
#define WHITE		(RED | GREEN | BLUE)
//
//
//#define _RENDERER_
//using namespace std;
//class Renderer
//{
//	float *m_outBuffer; // 3*width*height
//	float *m_zbuffer; // width*height
//	float *m_aliased_outBuffer; // 3*width*height
//	float *m_aliased_zbuffer; // 3*width*height
//	bool antiAliasing;
//	int m_width, m_height;
//	Fog* fog;
//	mat4 _projection;
//	mat4 _cTransform;
//	mat4 _oTransform;
//	mat4 _composition;
//	mat4 Mvp;
//	Light* activeLight;
//	vector<Light*> lights;
//	void CreateBuffers(int width, int height);
//	void CreateLocalBuffer();
//	bool drawNormal;
//	bool drawBound;
//	Camera* activeCamera;
//	void initMvp();
//	vec3 _kDiffuze;
//	vec3 _kAmbiant;
//	vec3 _kspecular;
//	GLfloat _shine;
//	int _color;
//	GLfloat Zdistance;
//	bool _cartoonize;
//	GLfloat _znear,_zfar;
//	//////////////////////////////
//	// openGL stuff. Don't touch.
//
//	GLuint gScreenTex;
//	GLuint gScreenVtc;
//	void CreateOpenGLBuffer();
//	void InitOpenGLRendering();
//	//////////////////////////////
//	
//
//	class Line; // This is a forward decleration
//	void putPixel(int x, int y, GLfloat zIndex, vec3 colors);
//	GLfloat sumQuad(int x, int y, int color);
//	void doAntialiasing();
//protected:
//	float *getOutBuffer();
//	float *getZBuffer();
//	
//public:
//	Renderer();
//	Renderer(int width, int height);
//	~Renderer(void);
//	
//	bool getAntialiasing() { return antiAliasing; }
//	void setAntialiasing(bool val) { 
//		antiAliasing = val; 
//		initMvp();
//	};
//
//	void displayPixel(int x, int y);
//
//	bool isDrawNormal();
//	void setDrawnormal(bool _drawNormal);
//	bool isDrawBound();
//	void setDrawBound(bool _drawBound);
//	void Init();
//	void DrawTriangles(const vector<vec3>* vertices, const vector<vec3>* normals=NULL);
//	bool DrawTriangle( Face face,vec3 color,bool flat = false);
//	void SetCameraTransform(const mat4& cTransform);
//	void SetProjection(const mat4& projection);
//	mat4 getObjectMatrices() { return _oTransform; };
//	void SetObjectMatrices(const mat4& oTransform);
//	void SetObjectMatrices(const mat4& oTransform, const mat3& nTransform);
//	void setLight(Light* newLight){activeLight = newLight;}
//	void SwapBuffers();
//	void ClearColorBuffer();
//	void ClearDepthBuffer();
//	void SetDemoBuffer();
//	void drawLineByVectors(vec3 from ,vec3 to,bool normal = false);
//	void drawLineByVectors(vec3 from ,vec3 to,unsigned int color = 0xffffff);
//	//TODO draw triangle
//	bool plot(Face worldFace,Face frameFace,int x, int y, vec3 color,vec3 normal,vec3 baryCordinate,GLfloat zcordinate,GLfloat g=0.5); /* plot a single point */
//	void plotFrench(int x, int y, GLfloat zIndex);
//	void DrawTriangleFrech( Face face,vec3 color);
//	int getWidth() const; /* Return screen's width */
//	int getHeight() const; /* Returns screen's  height */
//	void addLine(Line& newLine);
//	void invalidate();
//	mat4 getProjection();
//	void drawAxis(mat4 axis = Translate(0,0,0));
//	vec2 calculateTransformation(vec4 relativePoint);
//	vec3 calculateMvpPCTransformation(vec4 worldPoint,bool mode=true);
//	void resetZBuffer();
//	bool checkZBuffer();
//	vec3 getLightFactorForPoint(GLfloat x,GLfloat y,GLfloat z,vec3& normal,Face& f);
//	void addLight(Light* l){lights.push_back(l);}
//	void addLights(vector<Light*> oldlights){lights = oldlights;}
//	vector<Light*> getLights(){return lights;}
//	void setActiveCamera(Camera* cam){activeCamera = cam;}
//	void setKAbmbiant(vec3 kambiant){_kAmbiant=kambiant;}
//	void setKDiffuze(vec3 kdiffuze){_kDiffuze=kdiffuze;}
//	void setKspecular(vec3 kspecular){_kspecular=kspecular;}
//	void setShine(GLfloat shine){_shine = shine;}
//	void setFog(vec3 fogColor,GLfloat density);
//	void clearFog()
//	{
//		delete fog;
//		fog = NULL;
//	}
//	void setColors(GLfloat val){_color=val;}
//	void setZdistance(GLfloat distance){Zdistance = distance;}
//	void setCartoon(bool val){_cartoonize=val;}
//	void setZNear(GLfloat val){_znear = val;}
//	void setZFar(GLfloat val){_zfar = val;}
//	void addFog();
//};
//
//#define delta_threshold 0.001
//
//#define horizontal_thresh 0.001
//#define vertical_thresh 100000
//
//class Renderer::Line
//{
///*	const double horizontal_thresh; // line slope smaller than this will be considered a horizontal line
//	const double vertical_thresh; // line slope larger than this will be considered a vertical line
//	*/
//	vec2 _p1;
//	vec2 _p2;
//	GLfloat slope;
//	bool horizontal;
//	bool vertical;
//	float *m_outBuffer; // 3*width*height
//	int m_width;
//	int m_height;
//	int m_color;
//	bool inverseSteps;
//	Renderer& m_renderer;
//	#define INDEX(width,x,y,c) ((x+y*width)*3+c)
//	#define INDEXOF(width,x,y) (x+y*width)
//
//	inline void plot(int x, int y) {
//		if(x<m_width&& y<m_height && x>=0 && y>=0)
//		{
//			m_outBuffer[INDEX(m_width,x,y,2)]=  m_color			& 0x0000ff;
//			m_outBuffer[INDEX(m_width,x,y,1)]= (m_color >> 8)	& 0x0000ff;
//			m_outBuffer[INDEX(m_width,x,y,0)]= (m_color >> 16)	& 0x0000ff;
//			/*int width = m_renderer.getWidth();
//			int height = m_renderer.getHeight();
//			float* z_buffer = m_renderer.getZBuffer();
//			z_buffer[INDEXOF(width,x,y)]=m_renderer._znear;*/
//		}
//	}
//
////	GLfloat calculateSlope(vec2& p1, vec2& p2, bool updateState = false);
//	void calculateSlope();
//	void drawLine(); // Regular bersenhaim algorithm
//
//	void draw(float *outBuffer);
//	void horizontalLine(float *outBuffer);
//	void verticalLine(float *outBuffer);
//public:
//	Line(vec2 p1, vec2 p2, int color);
//	Line(Renderer &renderer, vec2 p1, vec2 p2, int color);
//	
//	void draw();
//	~Line(void);
//};
