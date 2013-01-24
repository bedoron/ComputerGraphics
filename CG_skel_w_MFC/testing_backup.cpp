//
// Perspective view of a color cube using LookAt() and Ortho()
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.
//
#include "stdafx.h"
#include "vec.h"
#include "mat.h"
#include "InitShader.h"
typedef vec4  color4;
typedef vec4  point4;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 points[NumVertices];
color4 colors[NumVertices];

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
	point4( -0.5, -0.5,  0.5, 1.0 ),
	point4( -0.5,  0.5,  0.5, 1.0 ),
	point4(  0.5,  0.5,  0.5, 1.0 ),
	point4(  0.5, -0.5,  0.5, 1.0 ),
	point4( -0.5, -0.5, -0.5, 1.0 ),
	point4( -0.5,  0.5, -0.5, 1.0 ),
	point4(  0.5,  0.5, -0.5, 1.0 ),
	point4(  0.5, -0.5, -0.5, 1.0 )
};

// RGBA olors
color4 vertex_colors[8] = {
	color4( 0.0, 0.0, 0.0, 1.0 ),  // black
	color4( 1.0, 0.0, 0.0, 1.0 ),  // red
	color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
	color4( 0.0, 1.0, 0.0, 1.0 ),  // green
	color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
	color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
	color4( 1.0, 1.0, 1.0, 1.0 ),  // white
	color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
};

// Viewing transformation parameters

GLfloat radius = 1.0;
GLfloat theta = 0.0;
GLfloat phi = 0.0;

const GLfloat  dr = 5.0 * DegreesToRadians;

GLuint  model_view;  // model-view matrix uniform shader variable location

// Projection transformation parameters

GLfloat  left = -1.0, right = 1.0;
GLfloat  bottom = -1.0, top = 1.0;
GLfloat  zNear = -1.0, zFar = 1.0;

GLuint  projection; // projection matrix uniform shader variable location

//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices

int Index = 0;

void
quad( int a, int b, int c, int d )
{
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void
colorcube()
{
	quad( 1, 0, 3, 2 );
	quad( 2, 3, 7, 6 );
	quad( 3, 0, 4, 7 );
	quad( 6, 5, 1, 2 );
	quad( 4, 5, 6, 7 );
	quad( 5, 4, 0, 1 );
}

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
	colorcube();

	GLuint vao;
	glGenVertexArrays(1 ,&vao);
	glBindVertexArray( vao );

	GLuint buffer[2];
	glGenBuffers(2, buffer);
	glBindBuffer( GL_ARRAY_BUFFER, buffer[0]);
	glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	
	glBindBuffer( GL_ARRAY_BUFFER, buffer[1]);
	glBufferData( GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram( program );

	
	glBindBuffer( GL_ARRAY_BUFFER, buffer[0]);
	GLuint vPosition = glGetAttribLocation( program, "vPosition");
	glEnableVertexAttribArray(vPosition);

	
	glVertexAttribPointer( vPosition/*atrib*/, 4/*size*/, GL_FLOAT/*type*/,
							GL_FALSE/*normalized*/, 0/*stride*/, 0/*pointer*/);
	
	glBindBuffer( GL_ARRAY_BUFFER, buffer[1]);
	GLuint vColor= glGetAttribLocation( program, "vColor");
	glEnableVertexAttribArray(vColor);

	glVertexAttribPointer( vColor/*atrib*/, 4/*size*/, GL_FLOAT/*type*/,
		GL_FALSE/*normalized*/, 0/*stride*/, 0/*pointer*/);

	model_view = glGetUniformLocation( program, "model_view");
	projection = glGetUniformLocation( program, "projection");
	glEnable(GL_DEPTH_TEST);
	glClearColor(1,1,1,1);
}

//----------------------------------------------------------------------------

void
display( void )
{	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	point4 eye(1,1,1,1);
	point4 at(0,0,0,1);
	point4 up(0,1,0,1);
	mat4 mv = LookAt(eye,at,up);
	
	glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);

	mat4 p= Ortho(left,right,bottom,top,0,10);
	glUniformMatrix4fv(projection, 1, GL_TRUE, p);
	glDrawArrays( GL_TRIANGLES, 0, NumVertices);
	glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
	switch( key ) {
	case 033: // Escape Key
	case 'q': case 'Q':
		exit( EXIT_SUCCESS );
		break;
	}
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
reshape( int width, int height )
{
	
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize( 512, 512 );
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );
	glutCreateWindow( "Color Cube" );
	glewExperimental = GL_TRUE;
	glewInit();

	init();

	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutReshapeFunc( reshape );

	glutMainLoop();
	return 0;
}
