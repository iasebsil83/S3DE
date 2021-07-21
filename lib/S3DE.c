// ---------------- IMPORTATIONS ----------------

//openGL compatibility
#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <glut.h>
#endif



//standard
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <float.h>
#include <string.h>



//own header
#include "S3DE.h"












/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ S3DE [0.1.7] ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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

        The whole 2D engine of S3DE is made from S2DE (which is another
    library I made). You can check it out at :
                      https://github.com/iasebsil83/S2DE

        This implies that all you can do with S2DE is possible with S3DE too.
    You can use S3DE as a 3D engine, 2D engine or both as you want.
    BE CAREFUL ! Always add your plaks AFTER S3DE_init().

    31/07/2020 > [0.1.0] :
    - Creating the whole engine (functions, constants and default program).

    03/08/2020 > [0.1.1] :
    - Passing plaks allocation from simple pointer to linked list (one way).

    03/08/2020 > [0.1.2] :
    - Added Plaks <-> STL files conversion functions.
      (S3DE_addPlaksFromSTL() & S3DE_saveSTLfromPlaks())
    - Added DEBUG_ON and DEEP_DEBUG_ON precompilation parameters.

    13/08/2020 > [0.1.3] :
    - Added perspective (FOV).
    - Fixed all the reshape system.
    - Passing S3DE_angle from 3 extern values to only 2 static (X and Y).
      (Optimization of cos and sin operations)
    - Added space movements (S3DE_forward() & S3DE_shift()).
    - Added constant S3DE_BACKSCENE_LIMIT to render only plaks that are
      in front of you.

    22/11/2020 > [0.1.4] :
    - Fixed bug : diagonals appeared in 2D rectangles and quads.

    08/12/2020 > [0.1.5] :
    - Fixed bug : Mouse Y coordinate is inverted.
    - Modified S2DEL_reshape() :
      Now, S3DE_newWidth & S3DE_newHeight are set before
      S3DE_RESHAPE event, and then
      S3DE_width & S3DE_height are set.

    18/02/2021 > [0.1.6] :
    - Added anti-redefinition beacon in header.
    - Renamed S3DE_setTimedExecution() in S3DE_setTimer() to fit with S2DE.
    - Added keyreleased events.
    - Changed example program.

    22/03/2021 > [0.1.7] :
    - Added S3DE_MOUSE_SCROLL event.
    - Removed some useless importations.

    06/04/2021 > [0.1.8] :
    - Started 3D forms patch (plane, cuboid, cube).
    - Optimized render process with the implementation of S3DE_realPlaks.
      (More memory management, less processor time).
    - Partially fixed movement in S3DE_goStraight().

    BUGS : S3DE_goStraight() is only working in the basic 3D axes.
    NOTES : S3DE_addPlaksFromSTL() temporary colors plaks in 2 colors :
                - half the time in the given color
                - half the time in its oposite
            (Uncomment definition of S3DE_STL_BICOLOR_ENABLE to disable it.)
            This is also the case for S3DE_addPlaks_plane(),
                                      S3DE_addPlaks_cuboid()
                                      S3DE_addPlaks_cube()

    S3DE is now compatible with another library I made
    for PNG image manipulation : PNG.c/.h.
    It is available here :
            https://github.com/iasebsil83/C_PNG
    Use S3DE_imageRGBA() to draw image on screen.

    Contact     : i.a.sebsil83@gmail.com
    Youtube     : https://www.youtube.com/user/IAsebsil83
    GitHub repo : https://github.com/iasebsil83

    Let's Code !                                  By I.A.
******************************************************************************************

    LICENCE :

    S3DE
    Copyright (C) 2021  Sebastien SILVANO
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.

    If not, see <https://www.gnu.org/licenses/>.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */












// ---------------- INITIALIZATION ----------------

//global vars
static int S3DE_window               =   -1;
static int S3DE_timedExecution_delay =   -1;
static int S3DE_width_2              =    0;
static int S3DE_height_2             =    0;
static int* S3DE_colorBuffer         = NULL;
static double* S3DE_depthBuffer      = NULL;



//debug
//uncomment the line below to activate debug messages
//#define DEBUG_ON
//uncomment the line below to activate deep debug messages
//#define DEEP_DEBUG_ON



//basic attributes
static double S3DE_angleX = 0.0;
static double S3DE_angleY = 0.0;
static double S3DE_cosX   = 0.0;
static double S3DE_sinX   = 0.0;
static double S3DE_cosY   = 0.0;
static double S3DE_sinY   = 0.0;
xyz S3DE_position = {
	.x = 0.0,
	.y = 0.0,
	.z = 0.0
};
double S3DE_FOV = 0.0007;



//3D plaks (linked list)
struct PLAK{
	xyz points[3];
	int color;
	struct PLAK* next;
};
typedef struct PLAK plak;
static int S3DE_plaksNbr    = 0;
static plak* S3DE_plaks     = NULL; //plaks of the scene
static plak* S3DE_realPlaks = NULL; //same plaks with space rotation + position offset



//event variables
int S3DE_mouseState         = 0; //mouse
int S3DE_mouseButton        = 0;
int S3DE_mouseScroll        = 0;
int S3DE_mouseX             = 0;
int S3DE_mouseY             = 0;
int S3DE_keyState           = 0; //keyboard
unsigned short S3DE_key     = 0;
unsigned int S3DE_newWidth  = 0; //resize
unsigned int S3DE_newHeight = 0;
unsigned int S3DE_width     = 0;
unsigned int S3DE_height    = 0;

//event handler
extern void S3DE_event(int event);












// ---------------- 3D ENGINE ----------------

//space rotations & movements
double S3DE_getAngleX(){
	return S3DE_angleX;
}

double S3DE_getAngleY(){
	return S3DE_angleY;
}

void S3DE_addAngleX(double angle){
	S3DE_angleX += angle;

	//recalculate cos & sin
	S3DE_cosX = cos(S3DE_angleX);
	S3DE_sinX = sin(S3DE_angleX);
}

void S3DE_addAngleY(double angle){
	S3DE_angleY += angle;

	//recalculate cos & sin
	S3DE_cosY = cos(S3DE_angleY);
	S3DE_sinY = sin(S3DE_angleY);
}

void S3DE_goStraight(double step, char direction){
	switch(direction){

		//front - back
		case S3DE_FRONT:
			S3DE_position.z += step;
		break;
		case S3DE_BACK:
			S3DE_position.z -= step;
		break;

		//left - right
		case S3DE_LEFT:
			S3DE_position.x -= step;
		break;
		case S3DE_RIGHT:
			S3DE_position.x += step;
		break;

		//up - down
		case S3DE_UP:
			S3DE_position.y += step;
		break;
		case S3DE_DOWN:
			S3DE_position.y -= step;
		break;

		default:
			printf("RUNTIME ERROR > S3DE.c : S3DE_goStraight() : Unknown direction \"%i\".\n", (int)direction);
	}
}



//render engine
static xyz S3DE_real(xyz point){

	//temporary variables
	double x = point.x;
	double y = point.y;
	double z = point.z;

	//y rotation build
	point.x = z*S3DE_sinY + x*S3DE_cosY;
	      z = z*S3DE_cosY - x*S3DE_sinY;

	//x rotation build
	point.y = z*S3DE_sinX + y*S3DE_cosX;
	point.z = z*S3DE_cosX - y*S3DE_sinX;

	//perspective
	if(point.z > 0){
		double FOVcoef = expf(-S3DE_FOV*point.z);

		//x
		if(point.x > S3DE_width_2)
			point.x = (double)S3DE_width_2 + FOVcoef*(point.x-S3DE_width_2);
		else if(point.x < S3DE_width_2)
			point.x = (double)S3DE_width_2 - FOVcoef*(S3DE_width_2-point.x);

		//y
		if(point.y > S3DE_height_2)
			point.y = (double)S3DE_height_2 + FOVcoef*(point.y-S3DE_height_2);
		else if(point.y < S3DE_height_2)
			point.y = (double)S3DE_height_2 - FOVcoef*(S3DE_height_2-point.y);
	}

	return point;
}

static void S3DE_realAllPlaks(){
	plak* currentP  = S3DE_plaks;
	plak* currentRP = S3DE_realPlaks;
	for(int p=0; p < S3DE_plaksNbr; p++){

		//set current shifted plak points
		//1st point
		currentRP->points[0].x = currentP->points[0].x - S3DE_position.x;
		currentRP->points[0].y = currentP->points[0].y - S3DE_position.y;
		currentRP->points[0].z = currentP->points[0].z - S3DE_position.z;
		currentRP->points[0] = S3DE_real(currentRP->points[0]);

		//2nd point
		currentRP->points[1].x = currentP->points[1].x - S3DE_position.x;
		currentRP->points[1].y = currentP->points[1].y - S3DE_position.y;
		currentRP->points[1].z = currentP->points[1].z - S3DE_position.z;
		currentRP->points[1] = S3DE_real(currentRP->points[1]);

		//3rd point
		currentRP->points[2].x = currentP->points[2].x - S3DE_position.x;
		currentRP->points[2].y = currentP->points[2].y - S3DE_position.y;
		currentRP->points[2].z = currentP->points[2].z - S3DE_position.z;
		currentRP->points[2] = S3DE_real(currentRP->points[2]);

		//set next plak as current
		currentP  = currentP->next;
		currentRP = currentRP->next;
	}
}

static double S3DE_getRealZ(xyz p[3], int x,int y){ //get the z corresponding to the point (x,y,?) of the plak
	//                                              //with x and y as real coordinates on the screen
	//                                                                                                          ------->   ------->
	double a = (p[1].y - p[0].y)*(p[2].z - p[0].z) - (p[1].z - p[0].z)*(p[2].y - p[0].y); //vectorial product : p[0]p[1] ^ p[0]p[2]
	double b = (p[2].x - p[0].x)*(p[1].z - p[0].z) - (p[2].z - p[0].z)*(p[1].x - p[0].x);
	double c = (p[1].x - p[0].x)*(p[2].y - p[0].y) - (p[1].y - p[0].y)*(p[2].x - p[0].x);
	double d = -(a*p[0].x + b*p[0].y + c*p[0].z); //p[0] belongs to the plan

	//so we have plan's equation : a*x + b*y + c*z + d = 0 (useful to find the z corresponding to an x and y)
	return -1.0*(a*x + b*y + d)/c;
}

static int* S3DE_getLine(int x1,int y1, int x2,int y2){ //create a chain of coordinates following the patern :
	//get differences in x and y                        //    int 0   : chain length
	int difX = abs(x2-x1);                              //    int 1,2 : x and y coordinates of first pixel
	int difY = abs(y2-y1);                              //    int 3,4 : x and y coordinates of second pixel ...

	//void line
	if(!difX && !difY){
		int* line = malloc(4);

		//error case
		if(line == NULL){
			printf("FATAL ERROR > S3DE.c : S3DE_getLine() : Computer refuses to give more memory.\n");
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
			printf("FATAL ERROR > S3DE.c : S3DE_getLine() : Computer refuses to give more memory.\n");
			exit(EXIT_FAILURE);
		}

		//set length
		line[0] = difX;

		//init line vars
		int x   = x1;
		double y = y1;
		double stepX = (x2 > x1) ? 1 : -1;
		double stepY = (double)(y2-y1)/difX; //not using difY : sign is important !

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
			printf("FATAL ERROR > S3DE.c : S3DE_getLine() : Computer refuses to give more memory.\n");
			exit(EXIT_FAILURE);
		}

		//set length
		line[0] = difY;

		//init line vars
		double x = x1;
		int y   = y1;
		double stepX = (double)(x2-x1)/difY; //not using difY : sign is important !
		double stepY = (y2 > y1) ? 1 : -1;

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

static void S3DE_renderPlak(plak* rp){ //triangle projection on screen made with lines
	//get main line (the first one)
	int* line_01 = S3DE_getLine(
		rp->points[0].x, rp->points[0].y,
		rp->points[1].x, rp->points[1].y
	);

	//for each pixel in main line
	int jx,jy;
	for(int i=0; i < line_01[0]; i++){
		//get line from current pixel to third point
		int* line_i2 = S3DE_getLine(
			line_01[1+2*i],  line_01[2+2*i],
			rp->points[2].x, rp->points[2].y
		);

		//for each pixel in current line
		for(int j=0; j < line_i2[0]; j++){
			//get point j
			jx = line_i2[1+2*j];
			jy = line_i2[2+2*j];

			//if this pixel is inside the screen
			if(S3DE_inScreen(jx,jy)){

				//compare with currently stored in depth buffer
				double newZ = S3DE_getRealZ(rp->points, jx,jy);
				if(newZ > S3DE_BACKSCENE_LIMIT && newZ < S3DE_depthBuffer[ jy*S3DE_width + jx ]){

					//set new depth at this coordinate
					S3DE_depthBuffer[ jy*S3DE_width + jx ] = newZ;

					//set new color at this coordinate
					S3DE_colorBuffer[ jy*S3DE_width + jx ] = rp->color;
				}
			}
		}

		//free current line
		free(line_i2);
	}

	//free main line & realpoints
	free(line_01);
}












// ---------------- PLAKS MANAGEMENT ----------------

//basics
static plak* S3DE_getPlak(int index){
	//error cases
	if(index < 0 || index >= S3DE_plaksNbr){
		printf("RUNTIME ERROR > S3DE.c : S3DE_getPlak() : Incorrect plak index \"%i\".\n", index);
		return NULL;
	}

	//get indexed plak
	plak* current = S3DE_plaks;
	for(int p=0; p < index; p++)
		current = current->next;

	return current;
}

static plak* S3DE_getRealPlak(int index){
	//error cases
	if(index < 0 || index >= S3DE_plaksNbr){
		printf("RUNTIME ERROR > S3DE.c : S3DE_getRealPlak() : Incorrect plak index \"%i\".\n", index);
		return NULL;
	}

	//get indexed plak
	plak* current = S3DE_realPlaks;
	for(int p=0; p < index; p++)
		current = current->next;

	return current;
}

void S3DE_addPlak(
	double x0,double y0,double z0,
	double x1,double y1,double z1,
	double x2,double y2,double z2,
	int color //in RGBA format (see S3DE_setPixelRGBA())
){
	//create new plak
	plak* p  = malloc(sizeof(plak));
	plak* rp = malloc(sizeof(plak));

	//error case
	if(p == NULL || rp == NULL){
		printf("FATAL ERROR > S3DE.c : S3DE_addPlak() : Computer refuses to give more memory.\n");
		exit(EXIT_FAILURE);
	}

	//set new plak
	p->points[0].x = x0; p->points[0].y = y0; p->points[0].z = z0;
	p->points[1].x = x1; p->points[1].y = y1; p->points[1].z = z1;
	p->points[2].x = x2; p->points[2].y = y2; p->points[2].z = z2;
	p->color = color;
	p->next  = NULL;

	//set new realPlak
	rp->points[0].x = x0; rp->points[0].y = y0; rp->points[0].z = z0;
	rp->points[1].x = x1; rp->points[1].y = y1; rp->points[1].z = z1;
	rp->points[2].x = x2; rp->points[2].y = y2; rp->points[2].z = z2;
	rp->color = color;
	rp->next  = NULL;

	//add it to S3DE_plaks
	if(S3DE_plaksNbr == 0){ //this is the first plak
		S3DE_plaks     =  p;
		S3DE_realPlaks = rp;
	}else{
		S3DE_getPlak    (S3DE_plaksNbr-1)->next =  p;
		S3DE_getRealPlak(S3DE_plaksNbr-1)->next = rp;
	}

	S3DE_plaksNbr++;
}

void S3DE_delPlak(int index){
	//error cases
	if(index < 0 || index >= S3DE_plaksNbr){
		printf("RUNTIME ERROR > S3DE.c : S3DE_delPlak() : Incorrect plak index \"%i\".\n", index);
		return;
	}

	//first case
	if(index == 0){
		plak* newFirst;
		plak* newRealFirst;

		//alone case
		if(S3DE_plaksNbr == 1){
			newFirst     = NULL;
			newRealFirst = NULL;
		}else{
			newFirst     = S3DE_getPlak    (1);
			newRealFirst = S3DE_getRealPlak(1);
		}

		//free old first
		free(S3DE_plaks);
		free(S3DE_realPlaks);

		//attach newFirst
		S3DE_plaks     = newFirst;
		S3DE_realPlaks = newRealFirst;

	//other cases
	}else{
		plak* prevPlak     = S3DE_getPlak    (index-1);
		plak* prevRealPlak = S3DE_getRealPlak(index-1);
		plak* nextPlak     = (prevPlak->next    )->next;
		plak* nextRealPlak = (prevRealPlak->next)->next;

		//free indexed plak
		free(prevPlak->next    );
		free(prevRealPlak->next);

		//attach nextPlak to prevPlak
		prevPlak->next     = nextPlak;
		prevRealPlak->next = nextRealPlak;
	}
	S3DE_plaksNbr--;
}












// ---------------- PLAKS / STL FILE CONVERSIONS ----------------
static int S3DE_strcmpN(char* s1, char* s2, int N){
	//error cases
	if(s1 == NULL || s2 == NULL){
		printf("RUNTIME ERROR > S3DE.c : S3DE_strcmpN() : At least one string is NULL.\n");
		return 0;
	}
	if(strlen(s1) < N || strlen(s2) < N){
		printf("RUNTIME ERROR > S3DE.c : S3DE_strcmpN() : At least one string is too short (\"%s\" & \"%s\", %i characters needed).\n", s1,s2, N);
		return 0;
	}

	//compare the first N characters
	for(int c=0; c < N; c++)
		if(s1[c] != s2[c])
			return 0;
	return 1;
}

void S3DE_addPlaksFromSTL(char* fileName, int dx,int dy,int dz, int color, double scale){
	//debug message
	#ifdef DEBUG_ON
	printf("DEBUG > S3DE.c : S3DE_addPlaksFromSTL() : Starting adding plaks from STL file \"%s\"...\n", fileName);
	#endif

	//error case
	if(fileName == NULL){
		printf("RUNTIME ERROR > S3DE.c : S3DE_addPlaksFromSTL() : Unable to add plaks, file name is NULL.\n");
		return;
	}

	//file management
	FILE* f = fopen(fileName,"r");
	if(f == NULL){
		printf("RUNTIME ERROR > S3DE.c : S3DE_addPlaksFromSTL() : No file \"%s\" found.\n",fileName);
		return;
	}

	//get file length
	fseek(f, 0L, SEEK_END);
	int file_len = ftell(f);
	fseek(f, 0L, SEEK_SET); //<=> rewind(f);
	#ifdef DEBUG_ON
	printf("DEBUG > S3DE.c : S3DE_addPlaksFromSTL() : File length : %i\n", file_len);
	#endif

	//init text buffer
	char* text = malloc(file_len+1);
	if(text == NULL){
		printf("RUNTIME ERROR > S3DE.c : S3DE_addPlaksFromSTL() : Computer refuses to give more memory.\n");
		return;
	}
	for(int c=0; c < file_len; c++)
		text[c] = '\0';

	//read from file
	char* temp=fgets(text,file_len,f); //temp avoids compiler warnings
	temp+=fscanf(f,"%[^!]", text);
	fclose(f);
	if(text == NULL){
		printf("RUNTIME ERROR > S3DE.c : S3DE_addPlaksFromSTL() : File \"%s\" has no text.\n",fileName);
		return;
	}

	//prepare variables for analysis
	int line_start = 0;
	int vertexNbr  = 0; //0:(x1,y1,z1), 1:(x2,y2,z2), 2:(x3,y3,z3)
	int cooNbr     = 0; //0:x, 1:y, 2:z
	double x1 = 0;	double y1 = 0;	double z1 = 0;
	double x2 = 0;	double y2 = 0;	double z2 = 0;
	double x3 = 0;	double y3 = 0;	double z3 = 0;
	char current[2];
	current[0] = '\0';
	current[1] = '\0';
	char* doubleText = malloc(S3DE_TEXT_FOR_FLOAT);
	if(doubleText == NULL){
		printf("RUNTIME ERROR > S3DE.c : S3DE_addPlaksFromSTL() : Computer refuses to give more memory.\n");
		return;
	}
	for(int c=0; c < S3DE_TEXT_FOR_FLOAT; c++)
		doubleText[c] = '\0';

	//analysis
	for(int c=0; c < file_len; c++){
		if(text[c] == '\n'){ //analyse line (from text[line_start] until text[c-1] included)
			#ifdef DEEP_DEBUG_ON
			printf("DEEP DEBUG > S3DE.c : S3DE_addPlaksFromSTL() : Analysing line \"");
			for(int i=line_start; i < c; i++)
				printf("%c",text[i]);
			printf("\"\n");
			#endif

			//start plak registration
			if(S3DE_strcmpN(text+line_start,"  outer loop",12)){
				#ifdef DEEP_DEBUG_ON
				printf("DEEP DEBUG > S3DE.c : S3DE_addPlaksFromSTL() : Found plak\n");
				#endif

				vertexNbr = 1;
			}
			else if(S3DE_strcmpN(text+line_start,"  vertex ",9)){
				//debug message
				#ifdef DEEP_DEBUG_ON
				printf("DEEP DEBUG > S3DE.c : S3DE_addPlaksFromSTL() : Adding point %i.\n", vertexNbr);
				#endif

				switch(vertexNbr){
					case 1:
						//forward until the first number
						line_start += 9;

						//register first point
						for(int i=0; line_start+i <= c; i++){
							*current = text[line_start+i];

							#ifdef DEEP_DEBUG_ON
							printf("DEEP DEBUG > S3DE.c : S3DE_addPlaksFromSTL() : doubleText : %s.\n", doubleText);
							#endif

							if(*current == ' ' || *current == '\n'){
								//separation found
								switch(cooNbr){
									case 0: //x
										x1 = atof(doubleText)*scale + (double)dx;
										cooNbr++;
									break;
									case 1: //y
										y1 = atof(doubleText)*scale + (double)dy;
										cooNbr++;
									break;
									case 2: //z
										z1 = atof(doubleText)*scale + (double)dz;
										cooNbr=0;
									break;
								}

								//reset doubleText
								for(int f=0; f < S3DE_TEXT_FOR_FLOAT; f++)
									doubleText[f] = '\0';
							}else
								strcat(doubleText, current);

						}
						vertexNbr++;
					break;
					case 2:
						//forward until the first number
						line_start += 9;

						//register second point
						for(int i=0; line_start+i <= c; i++){
							*current = text[line_start+i];

							#ifdef DEEP_DEBUG_ON
							printf("DEEP DEBUG > S3DE.c : S3DE_addPlaksFromSTL() : doubleText : %s.\n", doubleText);
							#endif

							if(*current == ' ' || *current == '\n'){
								//separation found
								switch(cooNbr){
									case 0: //x
										x2 = atof(doubleText)*scale + (double)dx;
										cooNbr++;
									break;
									case 1: //y
										y2 = atof(doubleText)*scale + (double)dy;
										cooNbr++;
									break;
									case 2: //z
										z2 = atof(doubleText)*scale + (double)dz;
										cooNbr=0;
									break;
								}

								//reset doubleText
								for(int f=0; f < S3DE_TEXT_FOR_FLOAT; f++)
									doubleText[f] = '\0';
							}else
								strcat(doubleText, current);
						}
						vertexNbr++;
					break;
					case 3:
						//forward until the first number
						line_start += 9;

						//register third point
						for(int i=0; line_start+i <= c; i++){
							*current = text[line_start+i];

							#ifdef DEEP_DEBUG_ON
							printf("DEEP DEBUG > S3DE.c : S3DE_addPlaksFromSTL() : doubleText : %s.\n", doubleText);
							#endif

							if(*current == ' ' || *current == '\n'){
								//separation found
								switch(cooNbr){
									case 0: //x
										x3 = atof(doubleText)*scale + (double)dx;
										cooNbr++;
									break;
									case 1: //y
										y3 = atof(doubleText)*scale + (double)dy;
										cooNbr++;
									break;
									case 2: //z
										z3 = atof(doubleText)*scale + (double)dz;
										cooNbr=0;
									break;
								}

								//reset doubleText
								for(int f=0; f < S3DE_TEXT_FOR_FLOAT; f++)
									doubleText[f] = '\0';
							}else
								strcat(doubleText, current);
						}

						//terminate registration of the plak
						#ifdef S3DE_STL_BICOLOR_ENABLE
						if(S3DE_plaksNbr%2 == 0)
						#endif
							S3DE_addPlak(
								x1,y1,z1,
								x2,y2,z2,
								x3,y3,z3, color //regular color
							);
						#ifdef S3DE_STL_BICOLOR_ENABLE
						else
							S3DE_addPlak(
								x1,y1,z1,
								x2,y2,z2,
								x3,y3,z3, (~color)|255 //opposite color
							);
						#endif
						#ifdef DEBUG_ON
						printf("DEBUG > S3DE.c : S3DE_addPlaksFromSTL() : ");
						printf("Added Plak {\n");
						printf("    (%f,%f,%f),\n", x1,y1,z1);
						printf("    (%f,%f,%f),\n", x2,y2,z2);
						printf("    (%f,%f,%f)\n" , x3,y3,z3);
						printf("}.\n");
						#endif
						vertexNbr = 0;
					break;
				}
			}
			line_start = c+1;
		}
	}

	//free file & analysis data
	free(text);

	//debug message
	#ifdef DEBUG_ON
	printf("DEBUG > S3DE.c : S3DE_addPlaksFromSTL() : Successfully added plaks from STL file \"%s\" !\n", fileName);
	#endif
}

void S3DE_saveSTLfromPlaks(char* fileName, double scale){
	//error case
	if(fileName == NULL){
		printf("RUNTIME ERROR > S3DE.c : S3DE_saveSTLfromPlaks() : Unable to export plaks, file name is NULL.\n");
		return;
	}

	//file management
	FILE* f = fopen(fileName,"w");
	if(f == NULL){
		printf("RUNTIME ERROR > S3DE.c : S3DE_saveSTLfromPlaks() : Unable to create or edit file \"%s\".\n", fileName);
		return;
	}

	//export
	fprintf(f,"solid \n");
	xyz* current = NULL;
	for(int p=0; p < S3DE_plaksNbr; p++){
		current = S3DE_getPlak(p)->points;
		fprintf(f," facet normal %f %f %f\n", //                    ------------------->   ------------------->
			(double)(                          //vectorial product : current[0]current[1] ^ current[0]current[2]
				(current[1].y - current[0].y)*(current[2].z - current[0].z) -
				(current[1].z - current[0].z)*(current[2].y - current[0].y)
			) * scale,
			(double)(
				(current[2].x - current[0].x)*(current[1].z - current[0].z) -
				(current[2].z - current[0].z)*(current[1].x - current[0].x)
			) * scale,
			(double)(
				(current[1].x - current[0].x)*(current[2].y - current[0].y) -
				(current[1].y - current[0].y)*(current[2].x - current[0].x)
			) *scale
		);
		fprintf(f,"  outer loop\n");
		fprintf(f,"  vertex %f %f %f\n",
			(double)current[0].x * scale,
			(double)current[0].y * scale,
			(double)current[0].z * scale
		);
		fprintf(f,"  vertex %f %f %f\n",
			(double)current[1].x * scale,
			(double)current[1].y * scale,
			(double)current[1].z * scale
		);
		fprintf(f,"  vertex %f %f %f\n",
			(double)current[2].x * scale,
			(double)current[2].y * scale,
			(double)current[2].z * scale
		);
		fprintf(f,"  endloop\n");
		fprintf(f," endfacet\n\n");
	}
	fprintf(f,"endsolid \n");

	//terminate
	fflush(f);
	fclose(f);

	//debug message
	#ifdef DEBUG_ON
	printf("DEBUG > S3DE.c : S3DE_saveSTLfromPlaks() : Plaks have been exported into \"%s\".\n", fileName);
	#endif
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
			S3DE_event(S3DE_TIMER);
		}
	}
}



//display
static void S3DEL_display(){
	//clear screen
	glFlush();
	glClearColor(0.f,0.f,0.f,0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//reset color & depth buffers
	int index=0;
	for(int y=0; y < S3DE_height; y++){
		for(int x=0; x < S3DE_width; x++){
			S3DE_colorBuffer[index  ] = S3DE_SKYCOLOR;
			S3DE_depthBuffer[index++] = DBL_MAX;
		}
	}

	//apply space rotations & movements
	S3DE_realAllPlaks();

	//counter for rendered plaks
	#ifdef DEBUG_ON
	int renderedNbr=0;
	#endif

	//screen rendering
	plak* current = S3DE_realPlaks;
	for(int p=0; p < S3DE_plaksNbr; p++){
		if(
			//abs(current->points[0].x - S3DE_position.x) < S3DE_RENDER_DISTANCE && //disable this optimization rule because
			//abs(current->points[0].y - S3DE_position.y) < S3DE_RENDER_DISTANCE && //of S3DE_position is not always correct
			//abs(current->points[0].z - S3DE_position.z) < S3DE_RENDER_DISTANCE && //due to S3DE_goStraight() arrangements
			//abs(current->points[1].x - S3DE_position.x) < S3DE_RENDER_DISTANCE &&
			//abs(current->points[1].y - S3DE_position.y) < S3DE_RENDER_DISTANCE &&
			//abs(current->points[1].z - S3DE_position.z) < S3DE_RENDER_DISTANCE &&
			//abs(current->points[2].x - S3DE_position.x) < S3DE_RENDER_DISTANCE &&
			//abs(current->points[2].y - S3DE_position.y) < S3DE_RENDER_DISTANCE && //needs the WHOLE plak inside
			//abs(current->points[2].z - S3DE_position.z) < S3DE_RENDER_DISTANCE && //RENDER_DISTANCE zone
			(
				current->points[0].z > S3DE_BACKSCENE_LIMIT ||
				current->points[1].z > S3DE_BACKSCENE_LIMIT ||
				current->points[2].z > S3DE_BACKSCENE_LIMIT
			)
		){
			S3DE_renderPlak(current);

			//count how many plaks are rendered
			#ifdef DEBUG_ON
			renderedNbr++;
			#endif
		}

		//set next plak as current
		current = current->next;
	}

	#ifdef DEBUG_ON
	printf("DEBUG > S3DE.c : S3DE_display() : %i/%i plaks rendered.\n",renderedNbr,S3DE_plaksNbr);
	#endif

	//3D screen displaying
	S3DE_imageRGBA(
		0,0,
		S3DE_width,S3DE_height,
		S3DE_colorBuffer
	);

	//extern program display
	S3DE_event(S3DE_DISPLAY);
	glutSwapBuffers();
}



//keyboard
static void S3DEL_keyPressed(GLubyte g, int x,int y){
	S3DE_key = g;
	S3DE_keyState = S3DE_KEY_PRESSED;
	S3DE_event(S3DE_KEYBOARD);
}

static void S3DEL_keyPressed_special(int keyCode, int x,int y){
	S3DE_key = 256 + (unsigned char)keyCode;
	S3DE_keyState = S3DE_KEY_PRESSED;
	S3DE_event(S3DE_KEYBOARD);
}

static void S3DEL_keyReleased(GLubyte g, int x,int y){
	S3DE_key = g;
	S3DE_keyState = S3DE_KEY_RELEASED;
	S3DE_event(S3DE_KEYBOARD);
}

static void S3DEL_keyReleased_special(int keyCode, int x,int y){
	S3DE_key = 256 + (unsigned char)keyCode;
	S3DE_keyState = S3DE_KEY_RELEASED;
	S3DE_event(S3DE_KEYBOARD);
}



//mouse
static void S3DEL_mouseButton(int button, int state, int x,int y){
	S3DE_mouseX = x;
	S3DE_mouseY = S3DE_height - y;
	S3DE_mouseState = state;

	//scroll
	if(button == 3 || button == 4){
		if(state == S3DE_MOUSE_PRESSED){
			S3DE_mouseScroll = button;
			S3DE_event(S3DE_MOUSE_SCROLL);
		}
	}else{
		S3DE_mouseButton = button;
		S3DE_event(S3DE_MOUSE_CLICK);
	}
}

static void S3DEL_mouseMoved(int x,int y){
	S3DE_mouseX = x;
	S3DE_mouseY = S3DE_height - y;
	usleep(1);
	if(S3DE_mouseScroll == 0){
		S3DE_event(S3DE_MOUSE_MOVE);
	}else{
		S3DE_mouseScroll = 0;
	}
}



//window reshape
static void S3DEL_reshape(int newWidth,int newHeight){

	//raise event with old values
	S3DE_newWidth  = newWidth;
	S3DE_newHeight = newHeight;
	S3DE_event(S3DE_RESIZE);
	S3DE_width  = newWidth;
	S3DE_height = newHeight;

	//set new values
	S3DE_width    = newWidth;
	S3DE_width_2  = newWidth/2;
	S3DE_height   = newHeight;
	S3DE_height_2 = newHeight/2;

	//reallocate color & depth buffers
	free(S3DE_colorBuffer);
	free(S3DE_depthBuffer);
	S3DE_colorBuffer = malloc(S3DE_width*S3DE_height*4);
	S3DE_depthBuffer = malloc(S3DE_width*S3DE_height*8);
	if(S3DE_colorBuffer == NULL || S3DE_depthBuffer == NULL){
		printf("FATAL ERROR > S3DE.c : S3DE_addPlaksFromSTL() : Computer refuses to give more memory.\n");
		exit(EXIT_FAILURE);
	}

	//set buffer default values
	int index=0;
	for(int y=0; y < S3DE_height; y++){
		for(int x=0; x < S3DE_width; x++){
			S3DE_colorBuffer[index  ] = S3DE_SKYCOLOR;
			S3DE_depthBuffer[index++] = DBL_MAX;
		}
	}

	//reshape
	glViewport(0,0, newWidth,newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.f,(GLdouble)newWidth, 0.f,(GLdouble)newHeight, -1.f,1.f);
	glMatrixMode(GL_MODELVIEW);
	S3DE_refresh();
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

void S3DE_setThickness(double thickness){
	glPointSize(thickness);
	glLineWidth(thickness);
}

int S3DE_inScreen(int x,int y){
	return x >= 0 && x < S3DE_width && y >= 0 && y < S3DE_height;
}



//graphics
void S3DE_point(double x, double y){
	glBegin(GL_POINTS);
		glVertex2f(x,y);
	glEnd();
}

void S3DE_line(double x1,double y1, double x2,double y2){
	glBegin(GL_LINES);
		glVertex2f(x1,y1);
		glVertex2f(x2,y2);
	glEnd();
}

void S3DE_triangle(double x1,double y1, double x2,double y2, double x3,double y3, int filled){
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

void S3DE_rectangle(double x1,double y1, double x2,double y2, int filled){
	if(filled){
		glBegin(GL_QUADS);
			glVertex2f(x1,y1);
			glVertex2f(x2,y1);
			glVertex2f(x2,y2);
			glVertex2f(x1,y2);
		glEnd();

		//GPU weakness : remove diagonals
		float thickness;
		glGetFloatv(GL_LINE_WIDTH, &thickness);
		glLineWidth(1.f);
		S3DE_line(x1,y1, x2,y2);
		glLineWidth(thickness);
	}else{
		S3DE_line(x1,y1, x2,y1);
		S3DE_line(x2,y1, x2,y2);
		S3DE_line(x2,y2, x1,y2);
		S3DE_line(x1,y2, x1,y1);
	}
}

void S3DE_quad(double x1,double y1, double x2,double y2, double x3,double y3, double x4,double y4, int filled){
	if(filled){
		glBegin(GL_QUADS);
			glVertex2f(x1,y1);
			glVertex2f(x2,y2);
			glVertex2f(x3,y3);
			glVertex2f(x4,y4);
		glEnd();

		//GPU weakness : remove diagonals
		float thickness;
		glGetFloatv(GL_LINE_WIDTH, &thickness);
		glLineWidth(1.f);
		S3DE_line(x1,y1, x3,y3);
		glLineWidth(thickness);
	}else{
		S3DE_line(x1,y1, x2,y2);
		S3DE_line(x2,y2, x3,y3);
		S3DE_line(x3,y3, x4,y4);
		S3DE_line(x4,y4, x1,y1);
	}
}



//text
void S3DE_text(char* text, double size, double x,double y){
	//moving over display (use of the 3rd dimension)
	glPushMatrix();
	glTranslatef(x,y,0);
	glScalef(size,size,size);

	//error case
	if(text == NULL){
		printf("RUNTIME ERROR > S3DE.c : S3DE_text() : Text is NULL.\n");
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
void S3DE_setTimer(int ms){
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
void S3DE_init(int argc, char** argv, const char* name, unsigned int width,unsigned int height){
	//error case
	if(name == NULL){
		printf("RUNTIME ERROR > S3DE.c : S3DE_init() : Cannot init window, name is NULL.\n");
		return;
	}

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
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glPointSize(2.f);

	//math init
	S3DE_cosX = cos(S3DE_angleX);
	S3DE_sinX = sin(S3DE_angleX);
	S3DE_cosY = cos(S3DE_angleY);
	S3DE_sinY = sin(S3DE_angleY);

	//init color & depth buffers
	S3DE_width    = width;
	S3DE_width_2  = width/2;
	S3DE_height   = height;
	S3DE_height_2 = height/2;
	S3DE_colorBuffer = malloc(S3DE_width*S3DE_height*4);
	S3DE_depthBuffer = malloc(S3DE_width*S3DE_height*8);

	//error cases
	if(S3DE_colorBuffer == NULL || S3DE_depthBuffer == NULL){
		printf("RUNTIME ERROR > S3DE.c : S3DE_init() : Computer refuses to give memory for colorBuffer or depthBuffer.\n");
		return;
	}

	//set buffer default values
	int index=0;
	for(int y=0; y < S3DE_height; y++){
		for(int x=0; x < S3DE_width; x++){
			S3DE_colorBuffer[index  ] = S3DE_SKYCOLOR;
			S3DE_depthBuffer[index++] = DBL_MAX;
		}
	}

	#ifdef DEBUG_ON
	//show the number of plaks loaded in scene
	printf("DEBUG > S3DE.c : S3DE_init() : %i plaks loaded.\n",S3DE_plaksNbr);
	#endif

	//set local S3DE event handlers (S3DEL)
	glutDisplayFunc      (S3DEL_display            );
	glutKeyboardFunc     (S3DEL_keyPressed         );
	glutSpecialFunc      (S3DEL_keyPressed_special );
	glutKeyboardUpFunc   (S3DEL_keyReleased        );
	glutSpecialUpFunc    (S3DEL_keyReleased_special);
	glutMouseFunc        (S3DEL_mouseButton        );
	glutMotionFunc       (S3DEL_mouseMoved         );
	glutPassiveMotionFunc(S3DEL_mouseMoved         );
	glutReshapeFunc      (S3DEL_reshape            );
}



//start - stop
void S3DE_start(){
	glutMainLoop();
}

void S3DE_stop(){
	//free 3D engine memory
	free(S3DE_colorBuffer);
	free(S3DE_depthBuffer);
	for(int p=0; p < S3DE_plaksNbr; p++)
		S3DE_delPlak(S3DE_plaksNbr-1);

	//stop 2D engine
	glutDestroyWindow(S3DE_window);
	exit(EXIT_SUCCESS);
}












// ---------------- FORMS PATCH ----------------

//hexaedron
void S3DE_addPlaks_cuboid(
	int x, int y, int z,
	int width, int height, int depth,
	int color
){
	//lengths
	double w_2 = width /2;
	double h_2 = height/2;
	double d_2 = depth /2;

	//TEMP <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	int ncolor = ~color | 255;

/* ------------------------------------------------

                   Coordinates plan
                   ----------------

              w
         <-------->

      ^   E--------F   ^
    h |   |\       |\   \
      |   | \      | \   \
      v   G--\-----H  \   \ d
           \  \     \  \   \
            \  \     \  \   \      -->
             \  \     \  \   \      z
              \  A--------B   v     ^
               \ |      \ |          \
                \|       \|           \        -->
                 C--------D            X------> x
                                       |
        A(-w_2, -h_2, -d_2)            |
        B( w_2, -h_2, -d_2)            | -->
        C(-w_2,  h_2, -d_2)            v  y
        D( w_2,  h_2, -d_2)

        E(-w_2, -h_2,  d_2)
        F( w_2, -h_2,  d_2)
        G(-w_2,  h_2,  d_2)
        H( w_2,  h_2,  d_2)
------------------------------------------------ */



	//FRONT - BACK

	//front face
	S3DE_addPlak(
		x-w_2, y-h_2, z-d_2, //A
		x+w_2, y-h_2, z-d_2, //B
		x-w_2, y+h_2, z-d_2, //C
		color
	);
	S3DE_addPlak(
		x+w_2, y-h_2, z-d_2, //B
		x-w_2, y+h_2, z-d_2, //C
		x+w_2, y+h_2, z-d_2, //D
		ncolor
	);

	//back face
	S3DE_addPlak(
		x-w_2, y-h_2, z+d_2, //E
		x+w_2, y-h_2, z+d_2, //F
		x-w_2, y+h_2, z+d_2, //G
		color
	);
	S3DE_addPlak(
		x+w_2, y-h_2, z+d_2, //F
		x-w_2, y+h_2, z+d_2, //G
		x+w_2, y+h_2, z+d_2, //H
		ncolor
	);



	//UP - DOWN

	//up face
	S3DE_addPlak(
		x-w_2, y-h_2, z-d_2, //A
		x+w_2, y-h_2, z-d_2, //B
		x-w_2, y-h_2, z+d_2, //E
		color
	);
	S3DE_addPlak(
		x+w_2, y-h_2, z-d_2, //B
		x-w_2, y-h_2, z+d_2, //E
		x+w_2, y-h_2, z+d_2, //F
		ncolor
	);

	//down face
	S3DE_addPlak(
		x-w_2, y+h_2, z-d_2, //C
		x+w_2, y+h_2, z-d_2, //D
		x-w_2, y+h_2, z+d_2, //G
		color
	);
	S3DE_addPlak(
		x+w_2, y+h_2, z-d_2, //D
		x-w_2, y+h_2, z+d_2, //G
		x+w_2, y+h_2, z+d_2, //H
		ncolor
	);



	//LEFT - RIGHT

	//left face
	S3DE_addPlak(
		x-w_2, y+h_2, z-d_2, //C
		x-w_2, y+h_2, z+d_2, //G
		x-w_2, y-h_2, z-d_2, //A
		color
	);
	S3DE_addPlak(
		x-w_2, y+h_2, z+d_2, //G
		x-w_2, y-h_2, z-d_2, //A
		x-w_2, y-h_2, z+d_2, //E
		ncolor
	);

	//right face
	S3DE_addPlak(
		x+w_2, y+h_2, z-d_2, //D
		x+w_2, y-h_2, z-d_2, //B
		x+w_2, y+h_2, z+d_2, //H
		color
	);
	S3DE_addPlak(
		x+w_2, y-h_2, z-d_2, //B
		x+w_2, y+h_2, z+d_2, //H
		x+w_2, y-h_2, z+d_2, //F
		ncolor
	);
}
