#include "stdafx.h"
#include "CGApp.h"
#include <iostream>

using std::cout;

#ifdef MOO
void main(int *argc, char **argv) {
	CGApp app(CGApp::INITIAL_WIDTH, CGApp::INITIAL_HEIGHT);
	cout << "CG Course app\n";
	app.initialize(argc, argv);
	app.setCallbacks();
	app.run();
}
#endif

CGApp *CGApp::instance = 0;

CGApp::CGApp(int width, int height): _width(width), _height(height) { }


CGApp::~CGApp(void)
{
}

bool CGApp::initialize(int *argc, char **argv) {
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(INITIAL_X,INITIAL_Y);
	glutInitWindowSize(_width, _height);
	glutInitContextVersion(GL_VERSION_TO_USE);
	//glutInitContextFlags();

	return true;	
}

void CGApp::setCallbacks() {
	instance = this;
	glutDisplayFunc(&(CGApp::display));
}

void CGApp::run() {
	glutMainLoop();
}


void CGApp::display(void) {
	cout << "Display\n";
	instance->_display();
}

void CGApp::_display() {
	glClear(GL_COLOR_BUFFER_BIT);

}