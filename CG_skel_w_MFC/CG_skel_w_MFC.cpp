// CG_skel_w_MFC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CG_skel_w_MFC.h"
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
using std::exception;
using std::cerr;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define DEBUG_MODE
// The one and only application object

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"
#include "vec.h"
#include "mat.h"
#include "InitShader.h"
#include "Scene.h"
#include "Renderer.h"
#include <string>
#include "InputDialog.h"
#include "AddCamera.h"
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

#define MAXIMUM_MENUS 8

enum Mouse_Mode{m_Model,m_camera,m_light, m_pixel_debug};
enum MENU_ELEMENTS {
OBJECTS_NAMESPACE_BEGIN = 0,
OBJECTS_NAMESPACE_END = 999,
CAMERAS_NAMESPACE_BEGIN = 1000,
CAMERAS_NAMESPACE_END = 1999,
OBJECTS_OVERFLOW_BUFFER = 2500, /* if you got here you suck */
FILE_OPEN, MAIN_DEMO , MAIN_ABOUT , Main_BOUNDS 
, Main_Clear , Main_selectM , Main_Move_Interval, MAIN_DEBUG, OBJECT_REMOVE_ACTIVE, 
CAMERA_ADD, CAMERA_REMOVE_ACTIVE,CAMERA_SELECT_MODEL_AT, SELECT_OPERATION_FRAME,Main_Frustum,
Main_Ortho,Main_prespective,RenderCameras,AddCube,addLight,addFog, MENU_ANTIALIASING
};


#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4
void renderBitmapString(float x,float y, string text);
Scene *scene;
Renderer *renderer;
int mouseMode = m_Model;
CModelData model_win;
Frustum dlg_frustum;
PresModel dlg_pres;
AddCamera dlg_addcamera;
MoveInterval dlg_interval;
AddLight dlg_light;
AddFog dlg_fog;
int last_x,last_y;
int pressedX,pressedY;
bool lb_down,rb_down,mb_down;
bool boundBox=true;
bool normals=true;
float moveInterval=1;
bool ctrlPressed = false;


//----------------------------------------------------------------------------
// Callbacks

void display( void )
{
	//Call the scene and ask it to draw itself
}

void reshape( int width, int height )
{ // strech the image
	cerr << "new width is : " << width << " new height is : "<< height << "\n";
	scene->setRenderer(width,height);
}

void keyboard( unsigned char key, int x, int y )
{
	switch ( key )
	{
	case 033:
		exit( EXIT_SUCCESS );
		break;
	case '\t':
		{
		Model *next = scene->nextModel();
		if(next) {
			cout << next->getName() << " was selected\n";
		} else { cout << "No models in scene\n"; }
		break;
		}
	case 'f':
	{
		scene->pointCameraAt();		
		break;
	}
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
	
}
void keyboardSpecial(int key,int x,int y)
{
	mat4 translatetion(1);
	switch ( key )
	{
	case GLUT_KEY_RIGHT:	
		translatetion*=Translate(vec4(1*scene->getMoveInterval(),0,0,0));
		break;
	case GLUT_KEY_LEFT:	
			translatetion*=Translate(vec4(-1*scene->getMoveInterval(),0,0,0));
		break;
	case GLUT_KEY_DOWN:	
		if(glutGetModifiers() == GLUT_ACTIVE_ALT)
			translatetion*=Translate(vec4(0,0,-1*scene->getMoveInterval(),0));
		else
			translatetion*=Translate(vec4(0,-1*scene->getMoveInterval(),0,0));
		break;
	case GLUT_KEY_UP:
		if(glutGetModifiers() == GLUT_ACTIVE_ALT)
			translatetion*=Translate(vec4(0,0,1*scene->getMoveInterval(),0));
		else
			translatetion*=Translate(vec4(0,1*scene->getMoveInterval(),0,0));
		break;
	case 033:
		exit( EXIT_SUCCESS );
		break;

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
			scene->draw();
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
void mouse(int button, int state, int x, int y)
{
	//button = {GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON}
	//state = {GLUT_DOWN,GLUT_UP}

	//set down flags
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
		if(mouseMode==m_pixel_debug)
			renderer->displayPixel(x,y);		
		break;
	case GLUT_WHEEL_UP:
		scene->setZoom(1);
		break;
	case GLUT_WHEEL_DOWN:
		scene->setZoom(-1);
		break;
	}

	// add your code
}

void motion(int x, int y)
{
	//return; // nigga
	// calc difference in mouse movement
	int dx=x-last_x;
	int dy=y-last_y;
	// update last x,y
	last_x=x;
	last_y=y;
	// TODO: FOR DORON - this part should be revised
	vec4 mouse(y-pressedY,x-pressedX);
	float mouse_len = length(mouse);
	mouse = normalize(mouse);

	Model *model = scene->getActiveModel();
	if(0==model) return; /* Nothing to spin */
	
	mat4 objBase = model->getObjectTransform(); // TODO - get Transform from renderer
	vec4 u = normalize(renderer->calculateTransformation(objBase[0]));
	vec4 v = normalize(renderer->calculateTransformation(objBase[1]));
	vec4 w = normalize(renderer->calculateTransformation(objBase[3]));

	vec3 cu = cross(u, mouse);
	vec3 cv = cross(v, mouse);
	vec3 cw = cross(w, mouse);

	float sinu = length(cu);
	float sinv = length(cv);
	float sinw = length(cw);
	
#define SIGN(a) (((a)<0)?(-1):(1))
	float zu = SIGN(cu.z);
	float zv = SIGN(cv.z);
	float zw = SIGN(cw.z);
#undef SIGN

	//cout << "sinu " << sinu << ", sinv " << sinv << ", sinw " << sinw << "\n";
	//cout << "Zu " << cu.z << ", Zv "  << cv.z << ", Zw " << cw.z << "\n";


	// Find maximum
	float m = sinu;
    if (m < sinv) m = sinv;
    if (m < sinw) m = sinw;

	float stepping = 10;
	mat4 rotation(RotateX(dy)*RotateY(dx));
	
	switch ( mouseMode)
	{
	case m_Model:
		{
			objBase = objBase * rotation;
			model->setObjectTransform(objBase);
			scene->draw(mat4());
			break;
		}
	case m_camera:
		{
			vec4 eye = scene->getActiveCamera()->getEye() - scene->getActiveCamera()->getAt();
			vec4 newEye =RotateY(-dx/3)*RotateX(-dy/3)*eye;
			newEye += scene->getActiveCamera()->getAt();
			scene->getActiveCamera()->LookAt(newEye,scene->getActiveCamera()->getAt(),scene->getActiveCamera()->getUp());
			scene->draw();
			cerr << "dx is: " << dx/3 << "\n";
			cerr << "dy is: " << dy/3 << "\n";
			break;
		}
	case m_light:
		{
			scene->changeLightDirection(rotation);
			break;
		}
	}
	
	
}

void mainMenu(int id)
{
	initMenu(false);
	switch (id)
	{
	case MAIN_DEMO:
		scene->drawDemo();
		break;
	case MAIN_ABOUT:
		AfxMessageBox(_T("Computer Graphics"));
		break;

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
	case addLight:
		{
			dlg_light.ShowWindow(SW_SHOW);

			break;
		}
	case addFog:
		{
			dlg_fog.ShowWindow(SW_SHOW);
			break;
		}
	case MENU_ANTIALIASING:
		{
			scene->setAntialiasing(!scene->getAntialiasing());
			break;
		}
	}
	initMenu();
}

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
		int objectSelected = id - OBJECTS_NAMESPACE_BEGIN;
		scene->setActiveModel(objectSelected);
		scene->showModelWindow();
		std::cerr << "Setting object " << objectSelected << "\n";
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
		int cameraSelected = id - CAMERAS_NAMESPACE_BEGIN + 1;
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
	subMenus[0] = glutCreateMenu(mainMenu);
	// Attach sub menus
	glutAddSubMenu("Object", subMenus[1]);
	glutAddSubMenu("Camera", subMenus[2]);
	glutAddSubMenu("View",subMenus[3]);
	// Add current menu elements
	
	glutAddMenuEntry("clear models",Main_Clear);
	glutAddMenuEntry("Select Move Interval",Main_Move_Interval);
	if(scene->getRenderCamera())
		glutAddMenuEntry("Do not Render cameras",RenderCameras);
	else
		glutAddMenuEntry("Render cameras",RenderCameras);
	glutAddMenuEntry("Look at active model (f)",CAMERA_SELECT_MODEL_AT);
	glutAddMenuEntry("Add Cube",AddCube);
	glutAddMenuEntry("Add Light",addLight);
	glutAddMenuEntry("Add Fog",addFog);
	if(scene->getAntialiasing())
		glutAddMenuEntry("Disable antialiasing", MENU_ANTIALIASING);
	else 
		glutAddMenuEntry("Enable antialiasing", MENU_ANTIALIASING);

	glutAddMenuEntry("About",MAIN_ABOUT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	

	return subMenus[0];
}


void initMenu(bool destroy) // Redraw entire menu every time we add an element
{
	menuDraw(destroy);
}
//----------------------------------------------------------------------------


int my_main( int argc, char **argv )
{
	//----------------------------------------------------------------------------
	// Initialize window
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA| GLUT_DOUBLE);
	glutInitWindowSize( MAIN_WIDTH, MAIN_HEIGHT);
	glutInitContextVersion( 3,1 );
	glutInitContextProfile( GLUT_CORE_PROFILE );
	glutCreateWindow( "CG" );
	glewExperimental = GL_TRUE;
	glewInit();
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		/*		...*/
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	dlg_frustum.Create(Frustum::IDD);
	model_win.Create(CModelData::IDD);
	dlg_pres.Create(PresModel::IDD);
	dlg_addcamera.Create(AddCamera::IDD);
	

	renderer = new Renderer(MAIN_WIDTH,MAIN_HEIGHT);
	scene = new Scene(renderer, model_win);
	model_win.initColors();
	dlg_frustum.setScene(scene);
	dlg_pres.setScene(scene);
	dlg_addcamera.setScene(scene);
	dlg_interval.Create(MoveInterval::IDD);
	dlg_interval.setScene(scene);
	dlg_light.Create(AddLight::IDD);
	dlg_light.setScene(scene);
	dlg_fog.Create(AddFog::IDD);
	dlg_fog.setScene(scene);
	//----------------------------------------------------------------------------
	// Initialize Callbacks

	
	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutSpecialFunc(keyboardSpecial);
	glutMouseFunc( mouse );
	glutMotionFunc ( motion );
	glutReshapeFunc( reshape );
	
	initMenu();


	glutMainLoop();
	delete scene;
	delete renderer;
	return 0;
}

CWinApp theApp;

using namespace std;

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
		my_main(argc, argv );
	}
	
	
	return nRetCode;
}
void renderBitmapString(float x,float y, string text) 
{  
  int index =0;
  glRasterPos3f(x, y,0);
  for (string::iterator it = text.begin() ; it < text.end(); it++ ,index++) {
	
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *it);
  }
}