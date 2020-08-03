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












/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ S3DE [0.1.2] ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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

    03/08/2020 > [0.1.1] :
    - Passing plaks allocation from simple pointer to linked list (one way).

    03/08/2020 > [0.1.2] :
    - Added Plaks <-> STL files conversion functions
      (S3DE_addPlaksFromSTL() & S3DE_saveSTLfromPlaks())

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
static int* S3DE_colorBuffer = NULL;
static int* S3DE_depthBuffer = NULL;




//position, angle & skyColor 
xyz S3DE_position;
float S3DE_angle[3] = {0.f,0.f,0.f};
int S3DE_skyColor = 16777215; //RGBA color corresponding to (0,255,255,255) (perfect cyan)



//plaks
static int S3DE_plaksNbr = 0;
static plak* S3DE_plaks = NULL;



//event handlers
extern void S3DE_timedExecution();
extern void S3DE_display();
extern void S3DE_keyPressed(char key);
extern void S3DE_mousePressed(int x,int y, int state);
extern void S3DE_mouseMoved(int x,int y);
extern void S3DE_reshape(int newWidth,int newHeight);












// ---------------- 3D ENGINE ----------------

//space rotations
static xyz S3DE_real(xyz point){
	//centralizing to (0,0,0) from S3DE_position
	point.x += S3DE_position.x;
	point.y += S3DE_position.y;
	point.z += S3DE_position.z;

	//math init
	float cosX = cos(S3DE_angle[0]);
	float sinX = sin(S3DE_angle[0]);
	float cosY = cos(S3DE_angle[1]);
	float sinY = sin(S3DE_angle[1]);
	float x = point.x;
	float y = point.y;
	float z = point.z;

	//y rotation build
	point.x = z*sinY + x*cosY;
	point.z = z*cosY - x*sinY;

	//x rotation build
	z = point.z; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	point.y = z*sinX + y*cosX;
	point.z = z*cosX - y*sinX;

	return point;
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
static int* S3DE_getLine(int x1,int y1, int x2,int y2){ //create a chain of coordinates following the patern :
	//get differences in x and y                        //    int 0 : chain length
	int difX = abs(x2-x1);                              //    int 1,2 : x and y coordinates of first pixel
	int difY = abs(y2-y1);                              //    int 3,4 : x and y coordinates of second pixel ...

	//void line
	if(!difX && !difY){
		int* line = malloc(4);
		
		//error case
		if(line == NULL){
			printf("FATAL ERROR > S3DE.c : S3DE_getLine() : ");
			printf("Computer refuses to give more memory.\n");
			exit(EXIT_FAILURE);
		}

		//set length
		line[0] = 0;
		return line;
	}

	//non-void line
	if(difX >= difY){
		//prepare line
		int* line = malloc(4 + difX*8);

		//error case
		if(line == NULL){
			printf("FATAL ERROR > S3DE.c : S3DE_getLine() : ");
			printf("Computer refuses to give more memory.\n");
			exit(EXIT_FAILURE);
		}

		//set length
		line[0] = difX;

		//init line vars
		int x   = x1;
		float y = y1;
		float stepX = (x2 > x1) ? 1 : -1;
		float stepY = (float)(y2-y1)/difX; //not using difY : sign is important !

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

		//error case
		if(line == NULL){
			printf("FATAL ERROR > S3DE.c : S3DE_getLine() : ");
			printf("Computer refuses to give more memory.\n");
			exit(EXIT_FAILURE);
		}

		//set length
		line[0] = difY;

		//init line vars
		float x = x1;
		int y   = y1;
		float stepX = (float)(x2-x1)/difY; //not using difY : sign is important !
		float stepY = (y2 > y1) ? 1 : -1;

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

static void S3DE_render(plak* p){
	//get main line (the first one)
	int* line_01 = S3DE_getLine(
		p->points[0].x, p->points[0].y,
		p->points[1].x, p->points[1].y
	);

	//for each pixel in main line
	for(int i=0; i < line_01[0]; i++){

		//get line from current pixel to third point
		int* line_i2 = S3DE_getLine(
			line_01[1+2*i], line_01[2+2*i],
			p->points[2].x, p->points[2].y
		);

		//for each pixel in current line
		for(int j=0; j < line_i2[0]; j++){
			//if this pixel is inside the screen
			if(S3DE_inScreen(line_i2[1+2*j], line_i2[2+2*j])){

				//compare with currently stored in depth buffer
				float newZ = S3DE_getRealZ(p->points, line_i2[1+2*j], line_i2[2+2*j]);
				if(newZ < S3DE_depthBuffer[
					line_i2[2+2*j]*S3DE_width + line_i2[1+2*j]
				]){
					//set new depth at this coordinate
					S3DE_depthBuffer[
						line_i2[2+2*j]*S3DE_width + line_i2[1+2*j]
					] = newZ;

					//set new color at this coordinate
					S3DE_colorBuffer[
						line_i2[2+2*j]*S3DE_width + line_i2[1+2*j]
					] = p->color;
				}
			}
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
	int color //in RGBA format (see S3DE_setPixelRGBA())
){
	//create new plak
	plak* p = malloc(sizeof(plak));

	//error case
	if(p == NULL){
		printf("FATAL ERROR > S3DE.c : S3DE_addPlak() : ");
		printf("Computer refuses to give more memory.\n");
		exit(EXIT_FAILURE);
	}

	//set new plak
	p->points[0].x = x0; p->points[0].y = y0; p->points[0].z = z0;
	p->points[1].x = x1; p->points[1].y = y1; p->points[1].z = z1;
	p->points[2].x = x2; p->points[2].y = y2; p->points[2].z = z2;
	p->color = color;
	p->next = NULL;

	//add it to S3DE_plaks
	if(S3DE_plaksNbr == 0) //this is the first plak
		S3DE_plaks = p;
	else
		S3DE_getPlak(S3DE_plaksNbr-1)->next = p;

	S3DE_plaksNbr++;
}

void S3DE_delPlak(int index){
	if(index < 0 || index >= S3DE_plaksNbr){
		printf("RUNTIME ERROR > S3DE.c : S3DE_delPlak() : ");
		printf("Incorrect plak index \"%i\".\n", index);
		return;
	}

	//first case
	if(index == 0){
		plak* newFirst = S3DE_getPlak(1);

		//free old first
		free(S3DE_plaks);

		//attach newFirst
		S3DE_plaks = newFirst;

	//other cases
	}else{
		plak* prevPlak = S3DE_getPlak(index-1);
		plak* nextPlak = (prevPlak->next)->next;

		//free indexed plak
		free(prevPlak->next);

		//attach nextPlak to prevPlak
		prevPlak->next = nextPlak;
	}
	S3DE_plaksNbr--;
}

plak* S3DE_getPlak(int index){
	//error cases
	if(index < 0 || index >= S3DE_plaksNbr){
		printf("RUNTIME ERROR > S3DE.c : S3DE_getPlak() : ");
		printf("Incorrect plak index \"%i\".\n", index);
		return NULL;
	}

	//get indexed plak
	plak* current = S3DE_plaks;
	for(int p=0; p < index; p++)
		current = current->next;

	return current;
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

	//color & depth buffers reset
	int index=0;
	for(int y=0; y < S3DE_height; y++){
		for(int x=0; x < S3DE_width; x++){
			S3DE_colorBuffer[index  ] = S3DE_skyColor;
			S3DE_depthBuffer[index++] = -1;
		}
	}

	//screen rendering
	plak* current = S3DE_plaks;
	for(int p=0; p < S3DE_plaksNbr; p++){
		if(
			(current->points[0].x - S3DE_position.x) < S3DE_RENDER_DISTANCE &&
			(current->points[0].y - S3DE_position.y) < S3DE_RENDER_DISTANCE &&
			(current->points[0].z - S3DE_position.z) < S3DE_RENDER_DISTANCE &&
			(current->points[1].x - S3DE_position.x) < S3DE_RENDER_DISTANCE &&
			(current->points[1].y - S3DE_position.y) < S3DE_RENDER_DISTANCE &&
			(current->points[1].z - S3DE_position.z) < S3DE_RENDER_DISTANCE &&
			(current->points[2].x - S3DE_position.x) < S3DE_RENDER_DISTANCE &&
			(current->points[2].y - S3DE_position.y) < S3DE_RENDER_DISTANCE && //needs the WHOLE plak inside
			(current->points[2].z - S3DE_position.z) < S3DE_RENDER_DISTANCE && //RENDER_DISTANCE zone
			(
				S3DE_real(current->points[0]).z > 0 ||
				S3DE_real(current->points[1]).z > 0 ||
				S3DE_real(current->points[2]).z > 0
			)
		)
			S3DE_render(current);

		//set next plak as current
		current = current->next;
	}

	//3D screen displaying
	S3DE_imageRGBA(
		0,0,
		S3DE_width,S3DE_height,
		S3DE_colorBuffer
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

int S3DE_inScreen(int x,int y){
	return x >= 0 && x <= S3DE_width && y >= 0 && y <= S3DE_height;
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

	//error case
	if(text == NULL){
		printf("RUNTIME ERROR > S3DE.c : S3DE_text() : ");
		printf("Text is NULL.\n");
		return;
	}

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
	//error case
	if(name == NULL){
		printf("RUNTIME ERROR > S3DE.c : S3DE_init() : ");
		printf("Cannot init window, name is NULL.\n");
		return;
	}

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

	//init color & depth buffers
	S3DE_width  = width;
	S3DE_height = height;
	S3DE_colorBuffer = malloc(S3DE_width*S3DE_height*4);
	S3DE_depthBuffer = malloc(S3DE_width*S3DE_height*4);

	//error cases
	if(S3DE_colorBuffer == NULL || S3DE_depthBuffer == NULL){
		printf("RUNTIME ERROR > S3DE.c : S3DE_init() : ");
		printf("Computer refuses to give memory for colorBuffer or depthBuffer.\n");
		return;
	}

	//set buffer default values
	int index=0;
	for(int y=0; y < S3DE_height; y++){
		for(int x=0; x < S3DE_width; x++){
			S3DE_colorBuffer[index  ] = S3DE_skyColor;
			S3DE_depthBuffer[index++] = -1;
		}
	}
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
