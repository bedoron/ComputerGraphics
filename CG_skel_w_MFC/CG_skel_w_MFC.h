#pragma once

#include "resource.h"
#include <sstream>

#define MAIN_WIDTH	1000
#define MAIN_HEIGHT	600

#define SSTR( x ) dynamic_cast< std::ostringstream & >(( std::ostringstream() << std::dec << x ) ).str()

void display( void );
void reshape( int width, int height );
void keyboard( unsigned char key, int x, int y );
void keyboardSpecial(int key,int x,int y);
void mouse(int button, int state, int x, int y);
void fileMenu(int id);
void mainMenu(int id);
void initMenu(bool destror = false);
int menuDraw(bool destroy = false);