// ---------------- IMPORTATIONS ----------------

//compatibility
#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/CGLMacro.h>
	#include <GLUT/glut.h>		// Header File For The GLut Library
#else
	#include <glut.h>			// Header File For The GLut Library
	#include <X11/Xlib.h>
	#include <GL/glx.h>
#endif



//standard
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>



//own header
#include "S3DE.h"












/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ S3DE [0.1.0] ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                        Simple 3Dimensional Engine

    Developped using freeglut3 (or just GLUT), a graphical 2D/3D engine.

    To install glut on linux-apt systems, just install the package :
                               freeglut3-dev

        This engine is a simple way to access to 3D graphical
    fuctionnalities provided by some calculations and the GLUT C library
    itself.
        Altought GLUT has a 3D engine implemented inside, S3DE uses only the
    2D engine of GLUT and gives it a third dimension manually. This was made
    like this to make the engine the more SIMPLE.

        The whole 2D engine of S3DE is made from S3DE (which is another
    library I made). You can check it out at :
                      https://github.com/iasebsil83/S3DE

        This implies that all you can do with S2DE is possible with S3DE too.
    You can use S3DE as a 3D engine, 2D engine or both as you want.
    BE CAREFUL ! Always add your plaks AFTER S3DE_init().

    31/07/2020 > [0.1.0] :
    - Creating the whole engine (functions, constants and default program).

    BUGS : .
    NOTES : .

    Contact     : i.a.sebsil83@gmail.com
    Youtube     : https://www.youtube.com/user/IAsebsil83
    GitHub repo : https://github.com/iasebsil83

    Let's Code !                                  By I.A.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */












// ---------------- INITIALIZATION ----------------

//global vars
static int S3DE_window = -1;
static int S3DE_timedExecution_delay = -1;
static int S3DE_width  = 0;
static int S3DE_height = 0;
static int* S3DE_screen3D = NULL;




//position & angle
xyz S3DE_position;
float S3DE_angle[3] = {0.f,0.f,0.f};



//plaks
static int S3DE_plaksNbr = 0;
static xyz** S3DE_plaks = NULL;
static int* S3DE_plaksColor = NULL;



//event handlers
extern void S3DE_timedExecution();
extern void S3DE_display();
extern void S3DE_keyPressed(char key);
extern void S3DE_mousePressed(int x,int y, int state);
extern void S3DE_mouseMoved(int x,int y);
extern void S3DE_reshape(int newWidth,int newHeight);












// ---------------- 3D ENGINE ----------------

//space rotations
static xyz S3DE_real(xyz basic){
	//centralizing to (0,0,0) from S3DE_position
	basic.x += S3DE_position.x;
	basic.y += S3DE_position.y;
	basic.z += S3DE_position.z;

	//math init
	float cosX = cos(S3DE_angle[0]);
	float sinX = sin(S3DE_angle[0]);
	float cosY = cos(S3DE_angle[1]);
	float sinY = sin(S3DE_angle[1]);
	float x = basic.x;
	float y = basic.y;
	float z = basic.z;

	//y rotation build
	basic.x = z*sinY + x*cosY;
	basic.z = z*cosY - x*sinY;

	//x rotation build
	z = basic.z; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	basic.y = z*sinX + y*cosX;
	basic.z = z*cosX - y*sinX;

	return basic;
}

static float S3DE_getRealZ(xyz p[3], int x,int y){ //get the z corresponding to the point (x,y,?) of the plak with x and y as real coordinates on the screen
	//                                                                                                         ------->   ------->
	float a = (p[1].y - p[0].y)*(p[2].z - p[0].z) - (p[1].z - p[0].z)*(p[2].y - p[0].y); //vectorial product : p[0]p[1] ^ p[0]p[2]
	float b = (p[2].x - p[0].x)*(p[1].z - p[0].z) - (p[2].z - p[0].z)*(p[1].x - p[0].x);
	float c = (p[1].x - p[0].x)*(p[2].y - p[0].y) - (p[1].y - p[0].y)*(p[2].x - p[0].x);
	float d = -(a*p[0].x + b*p[0].y + c*p[0].z);           //p[0] belongs to the plan
	//so we have plan's equation : a*x + b*y + c*z + d = 0 (useful to find the z corresponding to an x and y)
	return -(a*x + b*y + d)/c;
}



//render engine
static int* S3DE_getLine(xyz X1, xyz X2){ //create a chain of coordinates following the patern :
	//get differences in x and y          //int 0 : chain length
	int difX = abs(X2.x-X1.x);            //int 1,2 : x and y coordinates of first pixel
	int difY = abs(X2.y-X1.y);            //int 3,4 : x and y coordinates of second pixel ...

	//void line
	if(!difX && !difY){
		int* line = malloc(4);
		line[0] = 0;
		return line;
	}

	//non-void line
	if(difX >= difY){
		//prepare line
		int* line = malloc(4 + difX*8);
		line[0] = difX; //line length

		//init line vars
		int x   = X1.x;
		float y = X1.y;
		float stepX = (X2.x > X1.x) ? 1 : -1;
		float stepY = (float)(X2.y-X1.y)/difX; //not using difY : sign is important !

		//for each coordinate
		for(int p=0; p < difX; p++){
			//store coordinate
			line[1+2*p] = x;
			line[2+2*p] = y;
			x += stepX;
			y += stepY;
		}

		return line;
	}else{
		//prepare line
		int* line = malloc(4 + difY*8);
		line[0] = difY; //line length

		//init line vars
		float x = X1.x;
		int y   = X1.y;
		float stepX = ((float)(X2.x-X1.x))/difY; //not using difY : sign is important !
		float stepY = (X2.y > X1.y) ? 1 : -1;

		//for each coordinate
		for(int p=0; p < difY; p++){
			//store coordinate
			line[1+2*p] = x;
			line[2+2*p] = y;
			x += stepX;
			y += stepY;
		}

		return line;
	}
	return NULL; //unreachable code
}

static void S3DE_render(xyz p[3]){
	//get main line (the first one)
	int* line_01 = S3DE_getLine(p[0],p[1]);

	//for each pixel in main line
	for(int i=0; i < line_01[0]; i++){

		//get line from current pixel to third point
		int* line_i2 = S3DE_getLine(p[0],p[1]);

		//for each pixel of current line
		for(int j=0; j < line_i2[0]; j++){
			//calculations
		}

		//free current line
		free(line_i2);
	}

	//free main line
	free(line_01);
}












// ---------------- PLAKS MANAGEMENT ----------------
void S3DE_addPlak(
	int x0,int y0,int z0,
	int x1,int y1,int z1,
	int x2,int y2,int z2,
	int color
){ //color in RGBA format
	//allocation
	S3DE_plaks = realloc(S3DE_plaks, S3DE_plaksNbr*sizeof(xyz*));
	S3DE_plaksColor = realloc(S3DE_plaksColor, S3DE_plaksNbr*4);

	//error cases
	if(S3DE_plaks == NULL || S3DE_plaksColor == NULL){
		printf("FATAL ERROR > S3DE.c : S3DE_addPlak() : ");
		printf("Computer refuses to give more memory.\n");
	}

	//set new plak
	S3DE_plaks[S3DE_plaksNbr] = malloc(3*sizeof(xyz));
	S3DE_plaks[S3DE_plaksNbr][0].x = x0;
	S3DE_plaks[S3DE_plaksNbr][0].y = y0;
	S3DE_plaks[S3DE_plaksNbr][0].z = z0;
	S3DE_plaks[S3DE_plaksNbr][1].x = x1;
	S3DE_plaks[S3DE_plaksNbr][1].y = y1;
	S3DE_plaks[S3DE_plaksNbr][1].z = z1;
	S3DE_plaks[S3DE_plaksNbr][2].x = x2;
	S3DE_plaks[S3DE_plaksNbr][2].y = y2;
	S3DE_plaks[S3DE_plaksNbr][2].z = z2;
	S3DE_plaksColor[S3DE_plaksNbr] = color;
	S3DE_plaksNbr++;
}

void S3DE_delPlak(int index){
	S3DE_plaksNbr--;

	//create new S3DE_plaks & S3DE_plaksColor
	xyz** new_plaks     = malloc(S3DE_plaksNbr*sizeof(xyz*));
	int* new_plaksColor = malloc(S3DE_plaksNbr*4);

	//for each plak
	int new_index=0;
	for(int p=0; p < S3DE_plaksNbr+1; p++){
		if(p == index)
			continue;

		//allocate and set new plak
		new_plaks[new_index] = malloc(3*sizeof(xyz));
		new_plaks[new_index][0].x = S3DE_plaks[p][0].x;
		new_plaks[new_index][0].y = S3DE_plaks[p][0].y;
		new_plaks[new_index][0].z = S3DE_plaks[p][0].z;
		new_plaks[new_index][1].x = S3DE_plaks[p][1].x;
		new_plaks[new_index][1].y = S3DE_plaks[p][1].y;
		new_plaks[new_index][1].z = S3DE_plaks[p][1].z;
		new_plaks[new_index][2].x = S3DE_plaks[p][2].x;
		new_plaks[new_index][2].y = S3DE_plaks[p][2].y;
		new_plaks[new_index][2].z = S3DE_plaks[p][2].z;

		//set new plaksColor
		new_plaksColor[new_index] = S3DE_plaksColor[p];

		new_index++;
	}

	//free old plaks & plaksColor
	for(int p=0; p < S3DE_plaksNbr+1; p++)
		free(S3DE_plaks[p]);
	free(S3DE_plaks);
	free(S3DE_plaksColor);

	//set new plaks & plaksColor
	S3DE_plaks = new_plaks;
	S3DE_plaksColor = new_plaksColor;
}












// ---------------- LOCAL EVENT HANDLERS (S3DEL) ----------------

//timed executions
static void S3DEL_timedExecution(int i){
	if(i < 0)
		S3DE_refresh();
	else{
		if(S3DE_timedExecution_delay >= 0){
			glutTimerFunc(
				(unsigned int)S3DE_timedExecution_delay,
				S3DEL_timedExecution,
				S3DE_timedExecution_delay
			);
			S3DE_timedExecution();
		}
	}
}



//display
static void S3DEL_display(){
	//clear screen
	glFlush();
	glClearColor(0.f,0.f,0.f,0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//3D screen reset
	int index=0;
	for(int y=0; y < S3DE_height; y++)
		for(int x=0; x < S3DE_width; x++)
			S3DE_screen3D[index++] = S3DE_setPixelRGBA(0,0,0, 255);

	//3D screen rendering
	for(int p=0; p < S3DE_plaksNbr; p++){
		if(
			(S3DE_plaks[p][0].x - S3DE_position.x) < S3DE_RENDER_DISTANCE &&
			(S3DE_plaks[p][0].y - S3DE_position.y) < S3DE_RENDER_DISTANCE &&
			(S3DE_plaks[p][0].z - S3DE_position.z) < S3DE_RENDER_DISTANCE &&
			(S3DE_plaks[p][1].x - S3DE_position.x) < S3DE_RENDER_DISTANCE &&
			(S3DE_plaks[p][1].y - S3DE_position.y) < S3DE_RENDER_DISTANCE &&
			(S3DE_plaks[p][1].z - S3DE_position.z) < S3DE_RENDER_DISTANCE &&
			(S3DE_plaks[p][2].x - S3DE_position.x) < S3DE_RENDER_DISTANCE &&
			(S3DE_plaks[p][2].y - S3DE_position.y) < S3DE_RENDER_DISTANCE && //needs the WHOLE plak inside
			(S3DE_plaks[p][2].z - S3DE_position.z) < S3DE_RENDER_DISTANCE && //RENDER_DISTANCE zone
			(
				S3DE_real(S3DE_plaks[p][0]).z > 0 ||
				S3DE_real(S3DE_plaks[p][1]).z > 0 ||
				S3DE_real(S3DE_plaks[p][2]).z > 0
			)
		)
			S3DE_render(S3DE_plaks[p]);
	}

	//3D screen displaying
	S3DE_imageRGBA(
		0,0,
		S3DE_width,S3DE_height,
		S3DE_screen3D
	);

	//extern program display
	S3DE_display();
	glutSwapBuffers();
}



//keyboard
static void S3DEL_keyPressed(GLubyte g, int x,int y){
	char c = (char)g;
	switch(c){
		case 8:
			S3DE_keyPressed(S3DE_KEY_BACKSPACE);
		break;
		case 9:
			S3DE_keyPressed(S3DE_KEY_TAB);
		break;
		case 13:
			S3DE_keyPressed(S3DE_KEY_ENTER);
		break;
		case 27:
			S3DE_keyPressed(S3DE_KEY_ESCAPE);
		break;
		case 127:
			S3DE_keyPressed(S3DE_KEY_DELETE);
		break;
		default:
			S3DE_keyPressed(c);
	}
}

static void S3DEL_keyPressed_special(int keyCode, int x,int y){
	switch(keyCode){
		case GLUT_KEY_F1:
			S3DE_keyPressed(S3DE_KEY_F1);
		break;
		case GLUT_KEY_F2:
			S3DE_keyPressed(S3DE_KEY_F2);
		break;
		case GLUT_KEY_F3:
			S3DE_keyPressed(S3DE_KEY_F3);
		break;
		case GLUT_KEY_F4:
			S3DE_keyPressed(S3DE_KEY_F4);
		break;
		case GLUT_KEY_F5:
			S3DE_keyPressed(S3DE_KEY_F5);
		break;
		case GLUT_KEY_F6:
			S3DE_keyPressed(S3DE_KEY_F6);
		break;
		case GLUT_KEY_F7:
			S3DE_keyPressed(S3DE_KEY_F7);
		break;
		case GLUT_KEY_F8:
			S3DE_keyPressed(S3DE_KEY_F8);
		break;
		case GLUT_KEY_F9:
			S3DE_keyPressed(S3DE_KEY_F9);
		break;
		case GLUT_KEY_F10:
			S3DE_keyPressed(S3DE_KEY_F10);
		break;
		case GLUT_KEY_F11:
			S3DE_keyPressed(S3DE_KEY_F11);
		break;
		case GLUT_KEY_F12:
			S3DE_keyPressed(S3DE_KEY_F12);
		break;
		case GLUT_KEY_UP:
			S3DE_keyPressed(S3DE_KEY_UP);
		break;
		case GLUT_KEY_DOWN:
			S3DE_keyPressed(S3DE_KEY_DOWN);
		break;
		case GLUT_KEY_LEFT:
			S3DE_keyPressed(S3DE_KEY_LEFT);
		break;
		case GLUT_KEY_RIGHT:
			S3DE_keyPressed(S3DE_KEY_RIGHT);
		break;
		case GLUT_KEY_PAGE_UP:
			S3DE_keyPressed(S3DE_KEY_PAGE_UP);
		break;
		case GLUT_KEY_PAGE_DOWN:
			S3DE_keyPressed(S3DE_KEY_PAGE_DOWN);
		break;
		case GLUT_KEY_HOME:
			S3DE_keyPressed(S3DE_KEY_HOME);
		break;
		case GLUT_KEY_END:
			S3DE_keyPressed(S3DE_KEY_END);
		break;
		case GLUT_KEY_INSERT:
			S3DE_keyPressed(S3DE_KEY_INSERT);
		break;
		default:
			S3DE_keyPressed(S3DE_KEY_UNDEFINED);
	}
}



//mouse
static void S3DEL_mousePressed(int button, int state, int x,int y){
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN)
				S3DE_mousePressed(x,y, S3DE_LEFT_PRESSED);
			else
				S3DE_mousePressed(x,y, S3DE_LEFT_RELEASED);
		break;
		case GLUT_MIDDLE_BUTTON:
			if(state == GLUT_DOWN)
				S3DE_mousePressed(x,y, S3DE_MIDDLE_PRESSED);
			else
				S3DE_mousePressed(x,y, S3DE_MIDDLE_RELEASED);
		break;
		case GLUT_RIGHT_BUTTON:
			if(state == GLUT_DOWN)
				S3DE_mousePressed(x,y, S3DE_RIGHT_PRESSED);
			else
				S3DE_mousePressed(x,y, S3DE_RIGHT_RELEASED);
		break;
	}
}

static void S3DEL_mouseMoved(int x,int y){
	S3DE_mouseMoved(x,y);
}

static void S3DEL_mouseMoved_passive(int x,int y){
	S3DE_mouseMoved(x,y);
}



//window reshape
static void S3DEL_reshape(int width,int height){
	glViewport(0,0, width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.f,(GLdouble)width, 0.f,(GLdouble)height, -1.f,1.f);
	glMatrixMode(GL_MODELVIEW);
	S3DE_reshape(width,height);
}












// ---------------- 2D UTILITIES ----------------

//useful
void S3DE_refresh(){
	glutPostRedisplay();
}

void S3DE_fullScreen(){
	glutFullScreen();
}

void S3DE_setColor(int r, int v, int b){
	glColor3f(r/255.f, v/255.f, b/255.f);
}

void S3DE_setThickness(float thickness){
	glPointSize(thickness);
	glLineWidth(thickness);
}



//graphics
void S3DE_point(float x, float y){
	glBegin(GL_POINTS);
		glVertex2f(x,y);
	glEnd();
}

void S3DE_line(float x1,float y1, float x2,float y2){
	glBegin(GL_LINES);
		glVertex2f(x1,y1);
		glVertex2f(x2,y2);
	glEnd();
}

void S3DE_triangle(float x1,float y1, float x2,float y2, float x3,float y3, int filled){
	if(filled){
		glBegin(GL_TRIANGLES);
			glVertex2f(x1,y1);
			glVertex2f(x2,y2);
			glVertex2f(x3,y3);
		glEnd();
	}else{
		S3DE_line(x1,y1, x2,y2);
		S3DE_line(x2,y2, x3,y3);
		S3DE_line(x3,y3, x1,y1);
	}
}

void S3DE_rectangle(float x1,float y1, float x2,float y2, int filled){
	if(filled){
		glBegin(GL_QUADS);
			glVertex2f(x1,y1);
			glVertex2f(x2,y1);
			glVertex2f(x2,y2);
			glVertex2f(x1,y2);
		glEnd();
	}else{
		S3DE_line(x1,y1, x2,y1);
		S3DE_line(x2,y1, x2,y2);
		S3DE_line(x2,y2, x1,y2);
		S3DE_line(x1,y2, x1,y1);
	}
}

void S3DE_quad(float x1,float y1, float x2,float y2, float x3,float y3, float x4,float y4, int filled){
	if(filled){
		glBegin(GL_QUADS);
			glVertex2f(x1,y1);
			glVertex2f(x2,y2);
			glVertex2f(x3,y3);
			glVertex2f(x4,y4);
		glEnd();
	}else{
		S3DE_line(x1,y1, x2,y2);
		S3DE_line(x2,y2, x3,y3);
		S3DE_line(x3,y3, x4,y4);
		S3DE_line(x4,y4, x1,y1);
	}
}



//text
void S3DE_text(char* text, float size, float x,float y){
	//moving over display (use of the 3rd dimension)
	glPushMatrix();
	glTranslatef(x,y,0);
	glScalef(size,size,size);

	//write text character per character
	while(*text)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *text++);
	glPopMatrix();
}



//images
void S3DE_imageRGBA(int x,int y, int width,int height, int* data){
	glRasterPos2i(x,y);
	glDrawPixels(width,height, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, data);
}

int S3DE_setPixelRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a){
	return (r << 24) + (g << 16) + (b <<  8) + a;
}



//timed executions
void S3DE_setTimedExecution(int ms){
	if(S3DE_timedExecution_delay < 0 && ms >= 0)
		glutTimerFunc(
			(unsigned int)ms,
			S3DEL_timedExecution,
			ms
		);

	//set new timedExecution delay
	S3DE_timedExecution_delay = ms;
}












// ---------------- BASICS -----------------

//init
void S3DE_init(int argc, char** argv, const char* name, int width,int height){
	//compatibility
	#ifdef __linux__
		int majOGL;
		int minOGL;
		Display *dsp = XOpenDisplay(NULL);
		if(dsp != NULL){
			glXQueryVersion(dsp, &majOGL, &minOGL);
			XCloseDisplay(dsp);
		}
	#endif

	//init window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(width, height);
	S3DE_window = glutCreateWindow(name);

	//some settings
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_NORMALIZE);

	//init position
	S3DE_position.x = 0;
	S3DE_position.y = 0;
	S3DE_position.z = 0;

	//init 3D screen
	S3DE_width  = width;
	S3DE_height = height;
	S3DE_screen3D = malloc(S3DE_width*S3DE_height*4);
	int index=0;
	for(int y=0; y < S3DE_height; y++)
		for(int x=0; x < S3DE_width; x++)
			S3DE_screen3D[index++] = S3DE_setPixelRGBA(0,0,0, 255);
}



//start - stop
void S3DE_start(){
	//set local S3DE event handlers (S3DEL)
	glutDisplayFunc      (S3DEL_display           );
	glutKeyboardFunc     (S3DEL_keyPressed        );
	glutSpecialFunc      (S3DEL_keyPressed_special);
	glutMouseFunc        (S3DEL_mousePressed      );
	glutMotionFunc       (S3DEL_mouseMoved        );
	glutPassiveMotionFunc(S3DEL_mouseMoved_passive);
	glutReshapeFunc      (S3DEL_reshape           );

	//launch event loop
	glutMainLoop();
}

void S3DE_stop(){
	glutDestroyWindow(S3DE_window);
	exit(EXIT_SUCCESS);
}
