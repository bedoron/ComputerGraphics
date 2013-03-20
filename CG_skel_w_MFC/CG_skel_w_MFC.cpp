// CG_skel_w_MFC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CG_skel_w_MFC.h"
<<<<<<< HEAD

=======
#include "Utils.h"
#include <exception>
#include "WindowExample.h"
#include "ModelData.h"
#include "Frustum.h"
#include "PresModel.h"
#include "MoveInterval.h"
#include "Vertex.h"
#include "AvlTree.h"
#include "AddLight.h"
#include "AddFog.h"
#include "Shader.h"
using std::exception;
using std::cerr;
>>>>>>> refs/heads/final-hw3

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"
#include "vec.h"
#include "mat.h"
#include "InitShader.h"
#include "Scene.h"
#include <string>
<<<<<<< HEAD

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
=======
#include "InputDialog.h"
#include "AddCamera.h"
#include <map>

using std::map;
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

#define MAXIMUM_MENUS 8

enum Mouse_Mode{m_Model,m_camera,m_light, m_pixel_debug};
enum MENU_ELEMENTS {
OBJECTS_NAMESPACE_BEGIN = 0,
OBJECTS_NAMESPACE_END = 500,
CAMERAS_NAMESPACE_BEGIN = 501,
CAMERAS_NAMESPACE_END = 1000,
LIGHTS_NAMESPACE_BEGIN = 1001,
LIGHTS_NAMESPACE_END = 1012,
SHADERS_NAMESPACE_BEGIN = 1101,
SHADERS_NAMESPACE_END = 1130,
OBJECTS_OVERFLOW_BUFFER = 2500, /* if you got here you suck */
FILE_OPEN, MAIN_DEMO , MAIN_ABOUT , Main_BOUNDS 
, Main_Clear , Main_selectM , Main_Move_Interval, MAIN_DEBUG, OBJECT_REMOVE_ACTIVE, 
CAMERA_ADD, CAMERA_REMOVE_ACTIVE,CAMERA_SELECT_MODEL_AT, SELECT_OPERATION_FRAME,Main_Frustum,
Main_Ortho,Main_prespective,RenderCameras,AddCube,addLight,addFog, MENU_ANTIALIASING, ADD_TEXTURE,Animation, GLOBAL_AMBIENT, REMOVE_LIGHT
};
>>>>>>> refs/heads/final-hw3

#define FILE_OPEN 1
#define MAIN_DEMO 1
#define MAIN_ABOUT 2

Scene *scene;
<<<<<<< HEAD
Renderer *renderer;

=======
int mouseMode = m_Model;
CModelData model_win;
Frustum dlg_frustum;
PresModel dlg_pres;
AddCamera dlg_addcamera;
MoveInterval dlg_interval;
AddLight dlg_light;
AddFog dlg_fog;
>>>>>>> refs/heads/final-hw3
int last_x,last_y;
bool lb_down,rb_down,mb_down;
<<<<<<< HEAD
=======
bool boundBox=true;
bool normals=true;
float moveInterval=1;
bool ctrlPressed = false;

map<int, string> shader_id_to_name;
>>>>>>> refs/heads/final-hw3

//----------------------------------------------------------------------------
// Callbacks

void display( void )
{
<<<<<<< HEAD
//Call the scene and ask it to draw itself
=======
	//Call the scene and ask it to draw itself
	
	if(scene->isAnimated())
	{
		scene->draw();
		glutPostRedisplay();
	}

>>>>>>> refs/heads/final-hw3
}

void reshape( int width, int height )
{
//update the renderer's buffers
}

void keyboard( unsigned char key, int x, int y )
{
	switch ( key ) {
	case 033:
		exit( EXIT_SUCCESS );
		break;
	}
<<<<<<< HEAD
=======
	case 'c':
		mouseMode=m_camera;
		cerr << "camera is using mouse\n";
		break;
	case 'm':
		mouseMode=m_Model;
		cerr << "model is using mouse\n";
		break;
	case 'l':
		mouseMode = m_light;
		cerr << "light is using mouse\n";
		break;
	case 'd':
		mouseMode = m_pixel_debug;
		cerr << "Pixel debug mode, click anywhere to get pixel values\n";
		break;
	}
	
	if(key>='1' && key <= '9')
	{
		int number = key - '0';
		if(glutGetModifiers() == GLUT_ACTIVE_ALT)
		{
			if(!scene->setActiveCamera(number))
				cerr << "camera number " << number << " does not exist\n";
		}
	
	
	}
	
>>>>>>> refs/heads/final-hw3
}

<<<<<<< HEAD
=======
	}
	switch (mouseMode)
	{
	case m_Model:
		{
			scene->draw(translatetion);
			break;
		}
	case m_camera:
		{
			vec4 newEye = translatetion * scene->getActiveCamera()->getEye();
			scene->getActiveCamera()->LookAt(newEye,scene->getActiveCamera()->getAt(),scene->getActiveCamera()->getUp());
			scene->refreshActiveCamera();
			break;
		}
	case m_light:
		{
			scene->changeLightLocation(translatetion);
			scene->draw();
			break;
		}
	}

	
	ctrlPressed = false;
}
>>>>>>> refs/heads/final-hw3
void mouse(int button, int state, int x, int y)
{
	//button = {GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON}
	//state = {GLUT_DOWN,GLUT_UP}
	
	//set down flags
	switch(button) {
<<<<<<< HEAD
		case GLUT_LEFT_BUTTON:
			lb_down = (state==GLUT_UP)?0:1;
			break;
		case GLUT_RIGHT_BUTTON:
			rb_down = (state==GLUT_UP)?0:1;
			break;
		case GLUT_MIDDLE_BUTTON:
			mb_down = (state==GLUT_UP)?0:1;	
			break;
=======
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
		//if(mouseMode==m_pixel_debug)
		//	renderer->displayPixel(x,y);		
		break;
	case GLUT_WHEEL_UP:
		{
			if(glutGetModifiers()==GLUT_ACTIVE_ALT)
			{
				scene->draw(Scale(2));
			}
			else
				scene->setZoom(1);
			break;
		}
	case GLUT_WHEEL_DOWN:
		{
			if(glutGetModifiers()==GLUT_ACTIVE_ALT)
			{
				scene->draw(Scale(0.5));
			}
			else
				scene->setZoom(-1);
			break;
	}
>>>>>>> refs/heads/final-hw3
	}

	// add your code
}

void motion(int x, int y)
{
	// calc difference in mouse movement
	int dx=x-last_x;
	int dy=y-last_y;
	// update last x,y
	last_x=x;
	last_y=y;
<<<<<<< HEAD
}

void fileMenu(int id)
{
	switch (id)
	{
		case FILE_OPEN:
			CFileDialog dlg(TRUE,_T(".obj"),NULL,NULL,_T("*.obj|*.*"));
			if(dlg.DoModal()==IDOK)
			{
				std::string s((LPCTSTR)dlg.GetPathName());
				scene->loadOBJModel((LPCTSTR)dlg.GetPathName());
			}
			break;
=======

	Model *model = scene->getActiveModel();
	if(0==model) return; /* Nothing to spin */
	
	float stepping = 10;

	mat4 rotation(RotateX(dy)*RotateY(dx));
	
	switch ( mouseMode)
	{
	case m_Model:
		{
			model->setObjectTransform(model->getObjectTransform()*rotation);
			scene->draw(mat4());
			break;
		}
	case m_camera:
		{
			vec4 eye = scene->getActiveCamera()->getEye() - scene->getActiveCamera()->getAt();
			vec4 newEye =RotateY(-dx/3)*RotateX(-dy/3)*eye;
			newEye += scene->getActiveCamera()->getAt();
			scene->getActiveCamera()->LookAt(newEye,scene->getActiveCamera()->getAt(),scene->getActiveCamera()->getUp());
			scene->refreshActiveCamera(); // will also draw
			break;
		}
	case m_light:
		{
			scene->changeLightDirection(rotation);
			break;
		}
>>>>>>> refs/heads/final-hw3
	}
}

void mainMenu(int id)
{
	switch (id)
	{
	case MAIN_DEMO:
		scene->drawDemo();
		break;
	case MAIN_ABOUT:
		AfxMessageBox(_T("Computer Graphics"));
		break;
<<<<<<< HEAD
=======

	case SELECT_OPERATION_FRAME:
		glutSetMenu(1); // FIX THIS SOMEDAY
		if(scene->isModelFrame()) {
			scene->worldFrame();
			std::cerr << "Setting world frame\n";
			glutChangeToMenuEntry(SELECT_OPERATION_FRAME, "to Model frame", SELECT_OPERATION_FRAME);
		} else  {
			scene->modelFrame();
			std::cerr << "Setting model frame\n";
			glutChangeToMenuEntry(SELECT_OPERATION_FRAME, "to World frame", SELECT_OPERATION_FRAME);
		}
		std::cerr << "isModelFrame(): " << scene->isModelFrame() << "\n";
		break;
	case Main_Clear:

		{
			scene->clearScene();
			
			break;
		}

	case CAMERA_SELECT_MODEL_AT:
		{
			scene->pointCameraAt();
			break;
		}
	case Main_Move_Interval:
		{
			dlg_interval.ShowWindow(SW_SHOW); 
			break;
		}
	case RenderCameras:
		{
			scene->setRenderCamera(!scene->getRenderCamera());
			break;
		}
	case AddCube:
		{
			scene->addCube();
			scene->draw();
		break;
		}
	case addFog:
		{
			dlg_fog.ShowWindow(SW_SHOW);
			break;
		}
	case ADD_TEXTURE:
		{

			CFileDialog dlg(TRUE,_T(".png"),NULL,NULL,_T("*.png|*.*")); // WTF ?
			if(dlg.DoModal()==IDOK) {
				std::string p((LPCTSTR)dlg.GetPathName());
				std::string f((LPCTSTR)dlg.GetFileName());
				scene->addTexture(f, p);
				//scene->loadOBJModel((LPCTSTR)dlg.GetPathName(), (LPCTSTR)dlg.GetFileName());
			
				scene->draw();			
			}
			//TODO: add file chooser
			break;
		}
	case Animation:
		{
			scene->setAnimate(!scene->isAnimated());
		}
>>>>>>> refs/heads/final-hw3
	}
}

<<<<<<< HEAD
void initMenu()
{

	int menuFile = glutCreateMenu(fileMenu);
	glutAddMenuEntry("Open..",FILE_OPEN);
	glutCreateMenu(mainMenu);
	glutAddSubMenu("File",menuFile);
	glutAddMenuEntry("Demo",MAIN_DEMO);
=======
void objectMenuHandler(int id) { 
	initMenu(true);	// Destroy menu
	switch(id) {
	case FILE_OPEN: {
		CFileDialog dlg(TRUE,_T(".obj"),NULL,NULL,_T("*.obj|*.*")); // WTF ?
		if(dlg.DoModal()==IDOK) {
			std::string s((LPCTSTR)dlg.GetPathName());
			scene->loadOBJModel((LPCTSTR)dlg.GetPathName(), (LPCTSTR)dlg.GetFileName());
			
			scene->draw();
			
		}
	} break;
	case OBJECT_REMOVE_ACTIVE:
		scene->removeActiveModel();
		break;
	default:  // Object # id - OBJECT_NAMESPACE_BEGIN was selected
		if(id >= OBJECTS_NAMESPACE_BEGIN && id <= OBJECTS_NAMESPACE_END) {
			int objectSelected = id - OBJECTS_NAMESPACE_BEGIN;
			scene->setActiveModel(objectSelected);
			scene->showModelWindow();
			std::cerr << "Setting object " << objectSelected << "\n";
		} else if(id>= SHADERS_NAMESPACE_BEGIN && id <= SHADERS_NAMESPACE_END) { 
			int shaderSelected = id - SHADERS_NAMESPACE_BEGIN;
			string shader_name = shader_id_to_name[shaderSelected];
			try {
				scene->getActiveModel()->setShader(scene->getShader(shader_name));
				scene->draw();
			} catch(...) {
				cerr << "Couldn't select shader id " << shaderSelected << "\n";
			}
			//cerr << "Selecting shader " << shaderSelected << " with name " << shader_id_to_name[shaderSelected] << "\n";	
		}
		break;

	}
	initMenu();
}

void viewMenuHandler(int id)
{
	initMenu(true);
	switch (id)
	{
	case Main_Frustum:
	{
		dlg_frustum.setName("Set Frustum");
		dlg_frustum.setMode(false);
		dlg_frustum.ShowWindow(SW_SHOW);
		break;
	}
	case Main_Ortho:
	{
		dlg_frustum.setName("Set Ortho");
		dlg_frustum.setMode(true);
		dlg_frustum.ShowWindow(SW_SHOW);	
		break;
	}
	case Main_prespective:
		dlg_pres.ShowWindow(SW_SHOW);
	default:
		break;
	}
	initMenu();
}
void cameraMenuHandler(int id) { 
	cerr << "Camera menu got " << id << "\n";
	initMenu(true);
	switch(id) {
	case CAMERA_ADD:
		{
			std::cerr << "Add camera\n";
			dlg_addcamera.resetData();
			dlg_addcamera.setMode(true);
			dlg_addcamera.ShowWindow(SW_SHOW); 

		}
		break;
	case CAMERA_REMOVE_ACTIVE:
		std::cerr << "Remove active camera - not implemented\n";
		break;
	default: // Camera # id - CAMERA_NAMESPACE_BEGIN was selected
		int cameraSelected = id - CAMERAS_NAMESPACE_BEGIN + 1; // The +1 is a fix!
		cerr << "Trying to select camera " << cameraSelected << "\n";
		scene->setActiveCamera(cameraSelected); // Camera indices begin with 1
		scene->activateCamera();
		dlg_addcamera.setMode(false);
		dlg_addcamera.ShowWindow(SW_SHOW);
		dlg_addcamera.refreshCameraData();
		
		break;
	}
	initMenu();
}

void createLightsHandler(int id) {
	initMenu(true);
	switch(id) {
	case addLight:
		{
			dlg_light.DoModal();
			//dlg_light.ShowWindow(SW_SHOW);
			break;
		}
	case GLOBAL_AMBIENT: {
		vec4 oldAmbient = scene->getGlobalAmbience() * 255;
		CColorDialog	dlg;
		dlg.m_cc.Flags |= CC_RGBINIT | CC_FULLOPEN;
		dlg.m_cc.rgbResult = RGB(oldAmbient[0],oldAmbient[1],oldAmbient[2]);
		COLORREF		color;
		GLfloat r,g,b;
		if(dlg.DoModal() == IDOK) {
			color = dlg.GetColor();
			r = GetRValue(color);
			g = GetGValue(color);
			b = GetBValue(color);

			vec4 ambient(r/255, g/255, b/255, 1);
			scene->setGlobalAmbience(ambient);
		}
		break;
	}
	case REMOVE_LIGHT: {
		scene->removeActiveLight();
		break;
	}
	default:
		int lightSelected = id - LIGHTS_NAMESPACE_BEGIN;
		scene->setActiveLight(lightSelected);
		cerr << "Selected light " << lightSelected << "\n";
	};
	initMenu();
}

int createCamerasMenu() {
	int cameraMenu = glutCreateMenu(cameraMenuHandler); // Handler of Menu
	glutAddMenuEntry("Add & Activate", CAMERA_ADD);
	glutAddMenuEntry("Remove Active", CAMERA_REMOVE_ACTIVE);
	
	vector<string> names = scene->getCameraNames();
	for(vector<string>::iterator it = names.begin(); it != names.end(); ++it) {
		//cerr << "Camera " << (*it).c_str() << "\n";
		const char *ref = it->c_str();
		int id = (it - names.begin());
		glutAddMenuEntry(ref, CAMERAS_NAMESPACE_BEGIN + id);
	}

	return cameraMenu;
}

int createObjectsMenu() {
	int objectMenu = glutCreateMenu(objectMenuHandler); // Handler of Menu
	glutAddMenuEntry("Open...", FILE_OPEN);
	glutAddMenuEntry("Remove Active", OBJECT_REMOVE_ACTIVE);
	// Populate elements
	vector<string> names = scene->getNames();
	if(names.size()>0) {
		for(vector<string>::iterator it = names.begin(); it != names.end(); ++it) {
			glutAddMenuEntry((*it).c_str(), OBJECTS_NAMESPACE_BEGIN + it - names.begin());
		}
	} 
	names = scene->listShaders();
	shader_id_to_name.clear();
	for(vector<string>::iterator it = names.begin(); it != names.end(); ++it) {
		string entry = *it;
		glutAddMenuEntry(entry.c_str(), SHADERS_NAMESPACE_BEGIN + (it - names.begin()));
		if(' ' == entry[0])
			entry = entry.erase(0,2);
		shader_id_to_name[it - names.begin()] = entry; // Strip space
	}
	return objectMenu;
}
int createViewMenu()
{
	int viewMenu = glutCreateMenu(viewMenuHandler);
	glutAddMenuEntry("Set Frustom",Main_Frustum);
	glutAddMenuEntry("Set Ortho",Main_Ortho);
	glutAddMenuEntry("Set Prespective",Main_prespective);
	return viewMenu;
}
int createLightsMenu() {
	int viewMenu = glutCreateMenu(createLightsHandler);
	glutAddMenuEntry("Add Light",addLight);
	glutAddMenuEntry("Remove Active", REMOVE_LIGHT);
	glutAddMenuEntry("Global ambience",GLOBAL_AMBIENT);
	vector<string> names = scene->getLights();
	for(int i = 0; i < names.size(); ++i) {
		glutAddMenuEntry(names[i].c_str(), LIGHTS_NAMESPACE_BEGIN + i);
	}
	return viewMenu;
}


int menuDraw(bool destroy) {
	static int subMenus[MAXIMUM_MENUS] = { -1, -1, -1, -1, -1, -1, -1, -1 };

	if(subMenus[0]!=-1) {
		glutSetMenu(subMenus[0]);
		glutDetachMenu(GLUT_RIGHT_BUTTON);
	}
	for(int i=MAXIMUM_MENUS-1; i>= 0 ; --i) {
		if(subMenus[i]==-1) continue;
		glutDestroyMenu(subMenus[i]);
		subMenus[i] = -1;
	}
	if(destroy) return -1;
	// Generate sub menus 
	subMenus[1] = createObjectsMenu();
	subMenus[2] = createCamerasMenu();
	// Create main menu
	subMenus[3] = createViewMenu();
	subMenus[4] = createLightsMenu();
	subMenus[0] = glutCreateMenu(mainMenu);
	// Attach sub menus
	glutAddSubMenu("Object", subMenus[1]);
	glutAddSubMenu("Camera", subMenus[2]);
	glutAddSubMenu("Lights", subMenus[4]);
	glutAddSubMenu("View",subMenus[3]);
	// Add current menu elements
	
	glutAddMenuEntry("clear models",Main_Clear);
	glutAddMenuEntry("Select Move Interval",Main_Move_Interval);
	glutAddMenuEntry("Look at active model (f)",CAMERA_SELECT_MODEL_AT);
	//glutAddMenuEntry("Add Cube",AddCube);
	glutAddMenuEntry("Add Fog",addFog);
	glutAddMenuEntry("Add Texture", ADD_TEXTURE);
	glutAddMenuEntry("animation",Animation);
/*	if(scene->getAntialiasing())
		glutAddMenuEntry("Disable antialiasing", MENU_ANTIALIASING);
	else 
		glutAddMenuEntry("Enable antialiasing", MENU_ANTIALIASING);
		*/
>>>>>>> refs/heads/final-hw3
	glutAddMenuEntry("About",MAIN_ABOUT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
//----------------------------------------------------------------------------



int my_main( int argc, char **argv )
{
	//----------------------------------------------------------------------------
	// Initialize window
	glutInit( &argc, argv );
<<<<<<< HEAD
	glutInitDisplayMode( GLUT_RGBA| GLUT_DOUBLE);
	glutInitWindowSize( 400, 500 );
	glutInitContextVersion( 2, 0 );
=======
	//glutInitDisplayMode( GLUT_RGBA| GLUT_DOUBLE);
	glutInitWindowSize( MAIN_WIDTH, MAIN_HEIGHT);
	//glutInitContextVersion( 3,1 );
>>>>>>> refs/heads/final-hw3
	//glutInitContextProfile( GLUT_CORE_PROFILE );
	glutCreateWindow( "CG" );
	glewExperimental = GL_TRUE;
	
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		/*		...*/
	}
	glClearColor(0.5,0.5,0.5,0);
	//glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	err = glGetError();
	if( err != GL_NO_ERROR) {
		printf("Init error!\n");
		return -1;
	}

	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

<<<<<<< HEAD
	
	
	renderer = new Renderer(512,512);
	scene = new Scene(renderer);
	//----------------------------------------------------------------------------
	// Initialize Callbacks

=======
	scene = new Scene(model_win);
	model_win.initColors();
	dlg_frustum.setScene(scene);
	dlg_pres.setScene(scene);
	dlg_addcamera.setScene(scene);
	dlg_interval.Create(MoveInterval::IDD);
	dlg_interval.setScene(scene);
	//dlg_light.Create(AddLight::IDD); -- Modal windows can't live with create :(
	dlg_light.setScene(scene);
	dlg_fog.Create(AddFog::IDD);
	dlg_fog.setScene(scene);
	
	//------------------------------------------------------
	// Initialize Callbacks	
>>>>>>> refs/heads/final-hw3
	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mouse );
	glutMotionFunc ( motion );
	glutReshapeFunc( reshape );
	initMenu();
	

	glutMainLoop();
	delete scene;
	return 0;
}

CWinApp theApp;

using namespace std;
#define ALLOW_ORIGINAL_MAIN
#ifdef ALLOW_ORIGINAL_MAIN
int main( int argc, char **argv )
{
	int nRetCode = 0;
	
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		try {
			my_main(argc, argv );
		} catch (runtime_error& e) {
			MessageBox(NULL, _T(e.what()), _T("Runtime error"), MB_OK);
		} catch (exception& e) {
			MessageBox(NULL, _T(e.what()), _T("Exception error"), MB_OK);
		}
	}
	
	return nRetCode;
}
<<<<<<< HEAD
=======
#endif
void renderBitmapString(float x,float y, string text) 
{  
  int index =0;
  glRasterPos3f(x, y,0);
  for (string::iterator it = text.begin() ; it < text.end(); it++ ,index++) {
	
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *it);
  }
}
>>>>>>> refs/heads/final-hw3
