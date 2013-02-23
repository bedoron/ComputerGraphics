#pragma once
#include "GL/freeglut.h"

#define GL_VERSION_TO_USE	3, 0

class CGApp
{	
	static CGApp *instance;
	int _width;
	int _height;
	void _display();

public:
	static const int INITIAL_X = 0;
	static const int INITIAL_Y = 0;
	static const int INITIAL_WIDTH = 600;
	static const int INITIAL_HEIGHT = 500;

	CGApp(int width, int height);
	~CGApp(void);

	bool initialize(int *argc, char **argv);
	void setCallbacks();
	void run();

	static void display(void);
	
};

