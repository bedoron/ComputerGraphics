#pragma once
#include <vector>
#include "CG_skel_w_MFC.h"
#include "mat.h"
#include "GL/glew.h"
#include "Face.h"

#define BLACK		0x000000
#define RED			0xff0000
#define GREEN		0x00ff00
#define BLUE		0x0000ff
#define MAGENTA		(RED | BLUE)
#define YELLOW		(RED | GREEN)
#define AQUA		(GREEN | BLUE)
#define WHITE		(RED | GREEN | BLUE)


#define _RENDERER_
using namespace std;
class Renderer
{
	float *m_outBuffer; // 3*width*height
	float *m_zbuffer; // width*height
	int m_width, m_height;

	mat4 _projection;
	mat4 _cTransform;
	mat4 _oTransform;

	mat4 _composition;


	mat4 Mvp;


	void CreateBuffers(int width, int height);
	void CreateLocalBuffer();

	//////////////////////////////
	// openGL stuff. Don't touch.

	GLuint gScreenTex;
	GLuint gScreenVtc;
	void CreateOpenGLBuffer();
	void InitOpenGLRendering();
	//////////////////////////////
	bool drawNormal;
	bool drawBound;

	void initMvp();

	class Line; // This is a forward decleration
protected:
	float *getOutBuffer();
	
public:
	Renderer();
	Renderer(int width, int height);
	~Renderer(void);
	bool isDrawNormal();
	void setDrawnormal(bool _drawNormal);
	bool isDrawBound();
	void setDrawBound(bool _drawBound);
	void Init();
	void DrawTriangles(const vector<vec3>* vertices, const vector<vec3>* normals=NULL);
	void DrawFace(const Face& _face);
	void SetCameraTransform(const mat4& cTransform);
	void SetProjection(const mat4& projection);
	void SetObjectMatrices(const mat4& oTransform);
	void SetObjectMatrices(const mat4& oTransform, const mat3& nTransform);
	void SwapBuffers();
	void ClearColorBuffer();
	void ClearDepthBuffer();
	void SetDemoBuffer();
	void drawLineByVectors(vec3 from ,vec3 to,bool normal = false);
	void drawLineByVectors(vec3 from ,vec3 to,unsigned int color = 0xffffff);
	//TODO draw triangle
	void plot(int x, int y, int color); /* plot a single point */
	int getWidth() const; /* Return screen's width */
	int getHeight() const; /* Returns screen's  height */
	void addLine(Line& newLine);
	void invalidate();
	mat4 getProjection();
	void drawAxis(mat4 axis = Translate(0,0,0));
	vec2 calculateTransformation(vec4 relativePoint);
	vec2 calculateMvpPCTransformation(vec4 worldPoint); // Debug method
};

#define delta_threshold 0.001

#define horizontal_thresh 0.001
#define vertical_thresh 100000

class Renderer::Line
{
/*	const double horizontal_thresh; // line slope smaller than this will be considered a horizontal line
	const double vertical_thresh; // line slope larger than this will be considered a vertical line
	*/
	vec2 _p1;
	vec2 _p2;
	GLfloat slope;
	bool horizontal;
	bool vertical;
	float *m_outBuffer; // 3*width*height
	int m_width;
	int m_height;
	int m_color;
	bool inverseSteps;

	inline int INDEX(int width, int x, int y, int c) { return ((x+y*width)*3+c); }
	inline void plot(int x, int y) {
		if(x<m_width&& y<m_height && x>=0 && y>=0)
		{
			m_outBuffer[INDEX(m_width,x,y,2)]=  m_color			& 0x0000ff;
			m_outBuffer[INDEX(m_width,x,y,1)]= (m_color >> 8)	& 0x0000ff;
			m_outBuffer[INDEX(m_width,x,y,0)]= (m_color >> 16)	& 0x0000ff;
		}
	}

//	GLfloat calculateSlope(vec2& p1, vec2& p2, bool updateState = false);
	void calculateSlope();
	void drawLine(); // Regular bersenhaim algorithm

	void draw(float *outBuffer);
	void horizontalLine(float *outBuffer);
	void verticalLine(float *outBuffer);
public:
	Line(vec2 p1, vec2 p2, int color);
	Line(Renderer &renderer, vec2 p1, vec2 p2, int color);
	
	void draw();
	~Line(void);
};
