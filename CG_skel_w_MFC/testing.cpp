//pai
// Perspective view of a color cube using LookAt() and Ortho()
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.
//
#include "stdafx.h"
#include "vec.h"
#include "mat.h"
#include "InitShader.h"
#include "Utils.h"
#include "testing.h"
#include "MeshModel.h"
#include "Scene.h"
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"
typedef vec4  color4;
typedef vec4  point4;
#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4

 int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)
point4 eye(5,5,5,1);
point4 points[36];
color4 colors[36];
Scene* scene;
OBJItem item;
enum MENU_ELEMENTS {
	OBJECTS_NAMESPACE_BEGIN = 0,
	OBJECTS_NAMESPACE_END = 999,
	CAMERAS_NAMESPACE_BEGIN = 1000,
	CAMERAS_NAMESPACE_END = 1999,
	OBJECTS_OVERFLOW_BUFFER = 2500, /* if you got here you suck */
	FILE_OPEN, MAIN_DEMO , MAIN_ABOUT , Main_BOUNDS 
	, Main_Clear , Main_selectM , Main_Move_Interval, MAIN_DEBUG, OBJECT_REMOVE_ACTIVE, 
	CAMERA_ADD, CAMERA_REMOVE_ACTIVE,CAMERA_SELECT_MODEL_AT, SELECT_OPERATION_FRAME,Main_Frustum,
	Main_Ortho,Main_prespective,RenderCameras,AddCube,addLight,addFog, MENU_ANTIALIASING,menu_normalMapping,menu_Silhouette,menu_Phong,
	menu_toon,menu_texture,menu_enviroment
};


// Viewing transformation parameters

GLfloat radius = 1.0;
GLfloat theta = 0.0;
GLfloat phi = 0.0;
bool lb_down,rb_down,mb_down;
int last_x,last_y;
int pressedX,pressedY;
mat4 objectWorld = mat4(1);
const GLfloat  dr = 5.0 * DegreesToRadians;

GLuint  model_view;  // model-view matrix uniform shader variable location
GLuint  Camera_view;  // Camera-view matrix uniform shader variable location

// Projection transformation parameters

GLfloat  _left = -1.0, _right = 1.0;
GLfloat  _bottom = -1.0, _top = 1.0;
GLfloat  _zNear = -1.0, _zFar = 1.0;

GLuint  projection; // projection matrix uniform shader variable location

//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices

int Index = 0;

// OpenGL initialization
//void
//	init()
//{
//	
//	GLuint vao;
//	glGenVertexArrays(1 ,&vao);
//	glBindVertexArray( vao );
//	item.copyData();
//	vec4* points = item.getFacesAsArray();
//	vec4* colors = item.getNormalsAsArray();
//	NumVertices = item.getFacesSize()*3;
//
//	GLuint buffer[2];
//	glGenBuffers(2, buffer);
//	glBindBuffer( GL_ARRAY_BUFFER, buffer[0]);
//	glBufferData( GL_ARRAY_BUFFER,NumVertices* sizeof(vec4), points, GL_STATIC_DRAW);
//
//	glBindBuffer( GL_ARRAY_BUFFER, buffer[1]);
//	glBufferData( GL_ARRAY_BUFFER,NumVertices* sizeof(vec4), colors, GL_STATIC_DRAW);
//
//	GLuint program = InitShader("tmp.glsl", "fshader.glsl");
//	glUseProgram( program );
//
//
//	glBindBuffer( GL_ARRAY_BUFFER, buffer[0]);
//	GLuint vPosition = glGetAttribLocation( program, "vPosition");
//	glEnableVertexAttribArray(vPosition);
//
//
//	glVertexAttribPointer( vPosition/*atrib*/, 4/*size*/, GL_FLOAT/*type*/,
//		GL_FALSE/*normalized*/, 0/*stride*/, 0/*pointer*/);
//
//	glBindBuffer( GL_ARRAY_BUFFER, buffer[1]);
//	GLuint vColor= glGetAttribLocation( program, "vNormal");
//	glEnableVertexAttribArray(vColor);
//
//	glVertexAttribPointer( vColor/*atrib*/, 4/*size*/, GL_FLOAT/*type*/,
//		GL_FALSE/*normalized*/, 0/*stride*/, 0/*pointer*/);
//
//	model_view = glGetUniformLocation( program, "ModelView");
//	Camera_view = glGetUniformLocation(program,"CameraView");
//	projection = glGetUniformLocation( program, "Projection");
//	glEnable(GL_DEPTH_TEST);
//	glClearColor(1,1,1,1);
//}

//----------------------------------------------------------------------------

void
	display( void )
{	
	/*glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	point4 at(0,0,0,1);
	point4 up(0,1,0,1);
	mat4 cam = LookAt(eye,at,up);

	glUniformMatrix4fv(Camera_view, 1, GL_TRUE, cam);
	glUniformMatrix4fv(model_view, 1, GL_TRUE, objectWorld);

	mat4 p= Ortho(10*_left,10*_right,10*_bottom,10*_top,0,10);
	glUniformMatrix4fv(projection, 1, GL_TRUE, p);
	glDrawArrays( GL_TRIANGLES, 0, NumVertices);
	glutSwapBuffers();*/
		//Clear information from last draw
	scene->redraw();
	//glutSwapBuffers();

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
	case '6':
		scene->transformActiveModel(Translate(1,0,0));
		break;
	case '4':
		scene->transformActiveModel(Translate(-1,0,0));
		break;
	case '8':
		scene->transformActiveModel(Translate(0,1,0));
		break;
	case '2':
		scene->transformActiveModel(Translate(0,-1,0));
		break;
	case 't':
		{
			scene->nextTexture();
			glutPostRedisplay();
			break;
		}
	}

	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
	reshape( int width, int height )
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective

	//Set the camera perspective

	glLoadIdentity(); //Reset the camera

	gluPerspective(45.0,                  //The camera angle

		(double)width/ (double)height, //The width-to-height ratio

				   1.0,                   //The near z clipping coordinate

				   200.0);            
}
void mouse(int button, int state, int x, int y)
{
	last_x =x;
	last_y = y;
	switch(button) {
	case GLUT_LEFT_BUTTON:
		lb_down = (state==GLUT_UP)?0:1;	
		pressedX = x;
		pressedY = y;
		cout << "( " << x <<"," << y << ")\n";
		break;
	case GLUT_RIGHT_BUTTON:
		rb_down = (state==GLUT_UP)?0:1;
		break;
	case GLUT_MIDDLE_BUTTON:
		mb_down = (state==GLUT_UP)?0:1;	
		break;
	case GLUT_WHEEL_UP:
		if(glutGetModifiers() == GLUT_ACTIVE_ALT)
			scene->transformActiveModel(Scale(2,2,2));
		else
			scene->setZoom(1);
		break;
	case GLUT_WHEEL_DOWN:
		if(glutGetModifiers() == GLUT_ACTIVE_ALT)
			scene->transformActiveModel(Scale(0.5,0.5,0.5));
		else
			scene->setZoom(-1);
		break;
	}
	scene->redraw();
}

void motion(int x, int y)
{
	int dx=x-last_x;
	int dy=y-last_y;
	// update last x,y
	last_x=x;
	last_y=y;
	scene->transformActiveModel(RotateX(dy)*RotateY(dx));
	glutPostRedisplay();
}
void keyboardSpecial(int key,int x,int y)
{
		switch ( key )
	{
	case GLUT_KEY_RIGHT:	
			scene->transformActiveModel(Translate(1,0,0));
		break;
	case GLUT_KEY_LEFT:	
			scene->transformActiveModel(Translate(-1,0,0));
		break;
	case GLUT_KEY_DOWN:	
		{
			if(glutGetModifiers() == GLUT_ACTIVE_ALT)
				scene->transformActiveModel(Translate(0,0,-1));
			else
				scene->transformActiveModel(Translate(0,-1,0));
			break;
		}
	case GLUT_KEY_UP:
		{
			if(glutGetModifiers() == GLUT_ACTIVE_ALT)
				scene->transformActiveModel(Translate(0,0,1));
			else
				scene->transformActiveModel(Translate(0,1,0));
			break;
		}
	case 033:
		exit( EXIT_SUCCESS );
		break;

		}
		glutPostRedisplay();
}
void fileMenu(int id)
{
	switch (id)
	{
	case FILE_OPEN:
		{
			OPENFILENAME ofn;       // common dialog box structure
			char szFile[260];       // buffer for file name
			ZeroMemory( &ofn , sizeof( ofn));
			ofn.lStructSize = sizeof ( ofn );
			ofn.hwndOwner = NULL ;
			ofn.lpstrFile = szFile ;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof( szFile );
			ofn.lpstrFilter = "OBJ\0*.obj\0";
			ofn.nFilterIndex =1;
			ofn.lpstrFileTitle = NULL ;
			ofn.nMaxFileTitle = 0 ;
			ofn.lpstrInitialDir=NULL ;
			ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR ;
			// Display the Open dialog box. 

			if (GetOpenFileName(&ofn)==TRUE) 
			{
				std::string s(ofn.lpstrFile);
				item = Utils::getInstance().parseOBJ(s, s);

				MeshModel *model = new MeshModel(item);
				scene->loadOBJModel(model);
				glFlush();
			}
			break;
		}

	}
}

void mainMenu(int id)
{
	switch (id)
	{
	case MAIN_DEMO:

		break;
	case MAIN_ABOUT:
		AfxMessageBox(_T("Computer Graphics"));
		break;
	case menu_Phong:
		{
			scene->setProgramType(program_Phong);
			break;
		}
	case menu_normalMapping:
		{
			scene->setProgramType(program_NormalMapping);
			break;
		}
	case menu_Silhouette:
		{
			scene->setProgramType(program_Silhouette);
			break;
		}
	case menu_toon:
	{
		scene->setProgramType(program_toon);
		break;
	}
	case menu_texture:
	{
		scene->setProgramType(program_Texture);
		break;
	}
	case menu_enviroment:
	{
		OPENFILENAME ofn;       // common dialog box structure
			char szFile[260];       // buffer for file name
			ZeroMemory( &ofn , sizeof( ofn));
			ofn.lStructSize = sizeof ( ofn );
			ofn.hwndOwner = NULL ;
			ofn.lpstrFile = szFile ;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof( szFile );
			ofn.lpstrFilter = "PNG\0*.png\0";
			ofn.nFilterIndex =1;
			ofn.lpstrFileTitle = NULL ;
			ofn.nMaxFileTitle = 0 ;
			ofn.lpstrInitialDir=NULL ;
			ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR ;
			// Display the Open dialog box. 

			if (GetOpenFileName(&ofn)==TRUE) 
			{
				std::string s(ofn.lpstrFile);
				scene->loadPng(s.c_str(),GL_TEXTURE6,6);
				scene->setTextureNum(6);
				scene->setProgramType(program_enviroment);
				
			}
	
		break;
	}
	}

}

void initMenu()
{

	int menuFile = glutCreateMenu(fileMenu);
	glutAddMenuEntry("Open..",FILE_OPEN);
	glutCreateMenu(mainMenu);
	glutAddSubMenu("File",menuFile);
	glutAddMenuEntry("Phong",menu_Phong);
	glutAddMenuEntry("normal maing",menu_normalMapping);
	glutAddMenuEntry("Silhouette",menu_Silhouette);
	glutAddMenuEntry("Toon",menu_toon);
	glutAddMenuEntry("Texture",menu_texture);
	glutAddMenuEntry("Enviroment",menu_enviroment);
	glutAddMenuEntry("About",MAIN_ABOUT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
CWinApp theApp;

using namespace std;
int
	main( int argc, char **argv )
{
	

	glutInit( &argc, argv );
	//glutInitDisplayMode( GLUT_RGBA| GLUT_DOUBLE);
	glutInitWindowSize( 1000, 1000 );
	/*glutInitContextVersion( 3, 2 );*/
	//glutInitContextProfile( GLUT_CORE_PROFILE );
	glutCreateWindow( "Color Cube" );
	glewExperimental = GL_TRUE;
	glewInit();
	scene = new Scene();
	scene->LookAt(vec4(5,5,5,1),vec4(0,0,0,1),vec4(0,1,0,1));
	scene->setPrespective(45,1,1,50);
	
	
	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutReshapeFunc( reshape );
	glutSpecialFunc(keyboardSpecial);
	glutMouseFunc( mouse );
	glutMotionFunc ( motion );
	initMenu();
	glutMainLoop();
	return 0;
}
