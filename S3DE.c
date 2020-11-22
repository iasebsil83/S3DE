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
#include <float.h>
#include <string.h>



//own header
#include "S3DE.h"












/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ S3DE [0.1.3] ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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

    BUGS : S3DE_goStraight() is temporarily broken, however an alternative
               is made in S3DE_real().
    NOTES : S3DE_addPlaksFromSTL temporary colors plaks in 2 colors :
                - half the time in given color
                - half the time in its oposite
            This is just to make the scene lokking better.

    Contact     : i.a.sebsil83@gmail.com
    Youtube     : https://www.youtube.com/user/IAsebsil83
    GitHub repo : https://github.com/iasebsil83

    Let's Code !                                  By I.A.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */












// ---------------- INITIALIZATION ----------------

//global vars
static int S3DE_window = -1;
static int S3DE_timedExecution_delay = -1;
static int S3DE_width    = 0;
static int S3DE_width_2  = 0;
static int S3DE_height   = 0;
static int S3DE_height_2 = 0;
static int* S3DE_colorBuffer = NULL;
static float* S3DE_depthBuffer = NULL;



//debug
//uncomment the line below to activate debug messages
//#define DEBUG_ON
//uncomment the line below to activate deep debug messages
//#define DEEP_DEBUG_ON



//basic attributes
static float S3DE_angleX = 0.0;
static float S3DE_angleY = 0.0;
static float S3DE_cosX   = 0.0;
static float S3DE_sinX   = 0.0;
static float S3DE_cosY   = 0.0;
static float S3DE_sinY   = 0.0;
xyz S3DE_position = {
	.x = 0.0,
	.y = 0.0,
	.z = 0.0
};
float S3DE_FOV = 0.001;



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

//space rotations & movements
float S3DE_getAngleX(){
	return S3DE_angleX;
}

float S3DE_getAngleY(){
	return S3DE_angleY;
}

void S3DE_addAngleX(float angle){
	S3DE_angleX += angle;

	//recalculate cos & sin
	S3DE_cosX = cos(S3DE_angleX);
	S3DE_sinX = sin(S3DE_angleX);
}

void S3DE_addAngleY(float angle){
	S3DE_angleY += angle;

	//recalculate cos & sin
	S3DE_cosY = cos(S3DE_angleY);
	S3DE_sinY = sin(S3DE_angleY);
}

void S3DE_goStraight(float distance, char direction){
	switch(direction){
		case S3DE_FRONT:
			//temporarily broken
			//S3DE_position.x += distance*S3DE_sinY;
			//S3DE_position.y += distance*S3DE_cosY*S3DE_sinX;
			//S3DE_position.z += distance*S3DE_cosY*S3DE_cosX;
			S3DE_position.z += distance;
		break;
		case S3DE_BACK:
			//temporarily broken
			//S3DE_position.x -= distance*S3DE_sinY;
			//S3DE_position.y -= distance*S3DE_cosY*S3DE_sinX;
			//S3DE_position.z -= distance*S3DE_cosY*S3DE_cosX;
			S3DE_position.z -= distance;
		break;
		case S3DE_LEFT:
			//temporarily broken
			//S3DE_position.x -= distance*S3DE_cosY;
			//S3DE_position.y += distance*S3DE_sinY*S3DE_sinX;
			//S3DE_position.z += distance*S3DE_sinY*S3DE_cosX;
			S3DE_position.x -= distance;
		break;
		case S3DE_RIGHT:
			//temporarily broken
			//S3DE_position.x += distance*S3DE_cosY;
			//S3DE_position.y -= distance*S3DE_sinY*S3DE_sinX;
			//S3DE_position.z -= distance*S3DE_sinY*S3DE_cosX;
			S3DE_position.x += distance;
		break;
		case S3DE_UP:
			//temporarily broken
			//S3DE_position.x -= distance*S3DE_cosY;
			//S3DE_position.y += distance*S3DE_sinY*S3DE_cosX;
			//S3DE_position.z -= distance*S3DE_sinY*S3DE_sinX;
			S3DE_position.y += distance;
		break;
		case S3DE_DOWN:
			//temporarily broken
			//S3DE_position.x += distance*S3DE_cosY;
			//S3DE_position.y -= distance*S3DE_sinY*S3DE_cosX;
			//S3DE_position.z += distance*S3DE_sinY*S3DE_sinX;
			S3DE_position.y -= distance;
		break;
		default:
			printf("RUNTIME ERROR > S3DE.c : S3DE_goStraight() : ");
			printf("Unknown direction \"%i\".\n", (int)direction);
	}
}



//render engine
static xyz S3DE_real(xyz point){
	//centralizing to (0,0,0) from S3DE_position
	//temporarily broken
	//point.x -= S3DE_position.x;
	//point.y -= S3DE_position.y;
	//point.z -= S3DE_position.z;

	//temporary variables
	float x = point.x;
	float y = point.y;
	float z = point.z;

	//y rotation build
	point.x = z*S3DE_sinY + x*S3DE_cosY;
	      z = z*S3DE_cosY - x*S3DE_sinY;

	//x rotation build
	point.y = z*S3DE_sinX + y*S3DE_cosX;
	point.z = z*S3DE_cosX - y*S3DE_sinX;

	//S3DE_goStraight() alternative
	point.x -= S3DE_position.x;
	point.y -= S3DE_position.y;
	point.z -= S3DE_position.z;

	//perspective
	if(point.z > 0){
		float FOVcoef = expf(-S3DE_FOV*point.z);

		//x
		if(point.x > S3DE_width_2)
			point.x = (float)S3DE_width_2 + FOVcoef*(point.x-S3DE_width_2);
		else if(point.x < S3DE_width_2)
			point.x = (float)S3DE_width_2 - FOVcoef*(S3DE_width_2-point.x);

		//y
		if(point.y > S3DE_height_2)
			point.y = (float)S3DE_height_2 + FOVcoef*(point.y-S3DE_height_2);
		else if(point.y < S3DE_height_2)
			point.y = (float)S3DE_height_2 - FOVcoef*(S3DE_height_2-point.y);
	}

	return point;
}

static float S3DE_getRealZ(xyz p[3], int x,int y){ //get the z corresponding to the point (x,y,?) of the plak
	//                                             //with x and y as real coordinates on the screen
	//                                                                                                         ------->   ------->
	float a = (p[1].y - p[0].y)*(p[2].z - p[0].z) - (p[1].z - p[0].z)*(p[2].y - p[0].y); //vectorial product : p[0]p[1] ^ p[0]p[2]
	float b = (p[2].x - p[0].x)*(p[1].z - p[0].z) - (p[2].z - p[0].z)*(p[1].x - p[0].x);
	float c = (p[1].x - p[0].x)*(p[2].y - p[0].y) - (p[1].y - p[0].y)*(p[2].x - p[0].x);
	float d = -(a*p[0].x + b*p[0].y + c*p[0].z);           //p[0] belongs to the plan
	//so we have plan's equation : a*x + b*y + c*z + d = 0 (useful to find the z corresponding to an x and y)
	return -1.0*(a*x + b*y + d)/c;
}

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
	//get real points
	xyz* realPoints = malloc(3*sizeof(xyz));
	realPoints[0] = S3DE_real(p->points[0]);
	realPoints[1] = S3DE_real(p->points[1]);
	realPoints[2] = S3DE_real(p->points[2]);

	//get main line (the first one)
	int* line_01 = S3DE_getLine(
		realPoints[0].x, realPoints[0].y,
		realPoints[1].x, realPoints[1].y
	);

	//for each pixel in main line
	int jx,jy;
	for(int i=0; i < line_01[0]; i++){
		//get line from current pixel to third point
		int* line_i2 = S3DE_getLine(
			line_01[1+2*i] , line_01[2+2*i] ,
			realPoints[2].x, realPoints[2].y
		);

		//for each pixel in current line
		for(int j=0; j < line_i2[0]; j++){
			//get point j
			jx = line_i2[1+2*j];
			jy = line_i2[2+2*j];

			//if this pixel is inside the screen
			if(S3DE_inScreen(jx,jy)){

				//compare with currently stored in depth buffer
				float newZ = S3DE_getRealZ(realPoints, jx,jy);
				if(newZ > S3DE_BACKSCENE_LIMIT && newZ < S3DE_depthBuffer[ jy*S3DE_width + jx ]){

					//set new depth at this coordinate
					S3DE_depthBuffer[ jy*S3DE_width + jx ] = newZ;

					//set new color at this coordinate
					S3DE_colorBuffer[ jy*S3DE_width + jx ] = p->color;
				}
			}
		}

		//free current line
		free(line_i2);
	}

	//free main line & realpoints
	free(line_01);
	free(realPoints);
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
		plak* newFirst;

		//alone case
		if(S3DE_plaksNbr == 1)
			newFirst = NULL;
		else
			newFirst = S3DE_getPlak(1);

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












// ---------------- PLAKS / STL FILE CONVERSIONS ----------------
static int S3DE_strcmpN(char* s1, char* s2, int N){
	//error cases
	if(s1 == NULL || s2 == NULL){
		printf("RUNTIME ERROR > S3DE.c : S3DE_strcmpN() : ");
		printf("At least one string is NULL.\n");
		return 0;
	}
	if(strlen(s1) < N || strlen(s2) < N){
		printf("RUNTIME ERROR > S3DE.c : S3DE_strcmpN() : ");
		printf("At least one string is too short (\"%s\" & \"%s\", %i characters needed).\n", s1,s2, N);
		return 0;
	}

	//compare the first N characters
	for(int c=0; c < N; c++)
		if(s1[c] != s2[c])
			return 0;
	return 1;
}

void S3DE_addPlaksFromSTL(char* fileName, int dx,int dy,int dz, int color){
	//debug message
	#ifdef DEBUG_ON
	printf("DEBUG > S3DE.c : S3DE_addPlaksFromSTL() : ");
	printf("Starting adding plaks from STL file \"%s\"...\n", fileName);
	#endif

	//error case
	if(fileName == NULL){
		printf("RUNTIME ERROR > S3DE.c : S3DE_addPlaksFromSTL() : ");
		printf("Unable to add plaks, file name is NULL.\n");
		return;
	}

	//file management
	FILE* f = fopen(fileName,"r");
	if(f == NULL){
		printf("RUNTIME ERROR > S3DE.c : S3DE_addPlaksFromSTL() : ");
		printf("No file \"%s\" found.\n",fileName);
		return;
	}

	//get file length
	fseek(f, 0L, SEEK_END);
	int file_len = ftell(f);
	fseek(f, 0L, SEEK_SET); //<=> rewind(f);
	#ifdef DEBUG_ON
	printf("DEBUG > S3DE.c : S3DE_addPlaksFromSTL() : ");
	printf("File length : %i\n", file_len);
	#endif

	//init text buffer
	char* text = malloc(file_len+1);
	if(text == NULL){
		printf("RUNTIME ERROR > S3DE.c : S3DE_addPlaksFromSTL() : ");
		printf("Computer refuses to give more memory.\n");
		return;
	}
	for(int c=0; c < file_len; c++)
		text[c] = '\0';

	//read from file
	fgets(text,file_len,f);
	fscanf(f,"%[^!]", text);
	fclose(f);
	if(text == NULL){
		printf("RUNTIME ERROR > S3DE.c : S3DE_addPlaksFromSTL() : ");
		printf("File \"%s\" has no text.\n",fileName);
		return;
	}

	//prepare variables for analysis
	int line_start = 0;
	int vertexNbr  = 0; //0:(x1,y1,z1), 1:(x2,y2,z2), 2:(x3,y3,z3)
	int cooNbr     = 0; //0:x, 1:y, 2:z
	float x1=0; float y1=0; float z1=0;
	float x2=0; float y2=0; float z2=0;
	float x3=0; float y3=0; float z3=0;
	char* current = malloc(2);
	current[0] = '\0';
	current[1] = '\0';
	char* floatText = malloc(S3DE_TEXT_FOR_FLOAT);
	if(floatText == NULL){
		printf("RUNTIME ERROR > S3DE.c : S3DE_addPlaksFromSTL() : ");
		printf("Computer refuses to give more memory.\n");
		return;
	}
	for(int c=0; c < S3DE_TEXT_FOR_FLOAT; c++)
		floatText[c] = '\0';

	//analysis
	for(int c=0; c < file_len; c++){
		if(text[c] == '\n'){ //analyse line (from text[line_start] until text[c-1] included)
			#ifdef DEEP_DEBUG_ON
			printf("DEEP DEBUG > S3DE.c : S3DE_addPlaksFromSTL() : ");
			printf("Analysing line \"");
			for(int i=line_start; i < c; i++)
				printf("%c",text[i]);
			printf("\"\n");
			#endif

			//start plak registration
			if(S3DE_strcmpN(text+line_start,"  outer loop",12)){
				#ifdef DEEP_DEBUG_ON
				printf("DEEP DEBUG > S3DE.c : S3DE_addPlaksFromSTL() : ");
				printf("Found plak\n");
				#endif

				vertexNbr = 1;
			}
			else if(S3DE_strcmpN(text+line_start,"  vertex ",9)){
				//debug message
				#ifdef DEEP_DEBUG_ON
				printf("DEEP DEBUG > S3DE.c : S3DE_addPlaksFromSTL() : ");
				printf("Adding point %i.\n",vertexNbr);
				#endif

				switch(vertexNbr){
					case 1:
						//forward until the first number
						line_start += 9;

						//register first point
						for(int i=0; line_start+i <= c; i++){
							*current = text[line_start+i];

							#ifdef DEEP_DEBUG_ON
							printf("DEEP DEBUG > S3DE.c : S3DE_addPlaksFromSTL() : ");
							printf("floatText : %s.\n", floatText);
							#endif

							if(*current == ' ' || *current == '\n'){
								//separation found
								switch(cooNbr){
									case 0: //x
										x1 = atof(floatText)*S3DE_STL_AMPLIFICATION + (float)dx;
										cooNbr++;
									break;
									case 1: //y
										y1 = atof(floatText)*S3DE_STL_AMPLIFICATION + (float)dy;
										cooNbr++;
									break;
									case 2: //z
										z1 = atof(floatText)*S3DE_STL_AMPLIFICATION + (float)dz;
										cooNbr=0;
									break;
								}

								//reset floatText
								for(int f=0; f < S3DE_TEXT_FOR_FLOAT; f++)
									floatText[f] = '\0';
							}else
								strcat(floatText, current);

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
							printf("DEEP DEBUG > S3DE.c : S3DE_addPlaksFromSTL() : ");
							printf("floatText : %s.\n", floatText);
							#endif

							if(*current == ' ' || *current == '\n'){
								//separation found
								switch(cooNbr){
									case 0: //x
										x2 = atof(floatText)*S3DE_STL_AMPLIFICATION + (float)dx;
										cooNbr++;
									break;
									case 1: //y
										y2 = atof(floatText)*S3DE_STL_AMPLIFICATION + (float)dy;
										cooNbr++;
									break;
									case 2: //z
										z2 = atof(floatText)*S3DE_STL_AMPLIFICATION + (float)dz;
										cooNbr=0;
									break;
								}

								//reset floatText
								for(int f=0; f < S3DE_TEXT_FOR_FLOAT; f++)
									floatText[f] = '\0';
							}else
								strcat(floatText, current);
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
							printf("DEEP DEBUG > S3DE.c : S3DE_addPlaksFromSTL() : ");
							printf("floatText : %s.\n", floatText);
							#endif

							if(*current == ' ' || *current == '\n'){
								//separation found
								switch(cooNbr){
									case 0: //x
										x3 = atof(floatText)*S3DE_STL_AMPLIFICATION + (float)dx;
										cooNbr++;
									break;
									case 1: //y
										y3 = atof(floatText)*S3DE_STL_AMPLIFICATION + (float)dy;
										cooNbr++;
									break;
									case 2: //z
										z3 = atof(floatText)*S3DE_STL_AMPLIFICATION + (float)dz;
										cooNbr=0;
									break;
								}

								//reset floatText
								for(int f=0; f < S3DE_TEXT_FOR_FLOAT; f++)
									floatText[f] = '\0';
							}else
								strcat(floatText, current);
						}

						//terminate registration of the plak <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< temp
						if(S3DE_plaksNbr%2 == 0)
							S3DE_addPlak(
								x1,y1,z1,
								x2,y2,z2,
								x3,y3,z3, color //regular color
							);
						else
							S3DE_addPlak(
								x1,y1,z1,
								x2,y2,z2,
								x3,y3,z3, (~color)|255 //opposite color
							);
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
	free(current);
	free(text);

	//debug message
	#ifdef DEBUG_ON
	printf("DEBUG > S3DE.c : S3DE_addPlaksFromSTL() : ");
	printf("Successfully added plaks from STL file \"%s\" !\n", fileName);
	#endif
}

void S3DE_saveSTLfromPlaks(char* fileName){
	//error case
	if(fileName == NULL){
		printf("RUNTIME ERROR > S3DE.c : S3DE_saveSTLfromPlaks() : ");
		printf("Unable to export plaks, file name is NULL.\n");
		return;
	}

	//file management
	FILE* f = fopen(fileName,"w");
	if(f == NULL){
		printf("RUNTIME ERROR > S3DE.c : S3DE_saveSTLfromPlaks() : ");
		printf("Unable to create or edit file \"%s\".\n",fileName);
		return;
	}

	//export
	fprintf(f,"solid \n");
	xyz* current = NULL;
	for(int p=0; p < S3DE_plaksNbr; p++){
		current = S3DE_getPlak(p)->points;
		fprintf(f," facet normal %f %f %f\n", //                    ------------------->   ------------------->
			(float)(                          //vectorial product : current[0]current[1] ^ current[0]current[2]
				(current[1].y - current[0].y)*(current[2].z - current[0].z) -
				(current[1].z - current[0].z)*(current[2].y - current[0].y)
			) / S3DE_STL_AMPLIFICATION,
			(float)(
				(current[2].x - current[0].x)*(current[1].z - current[0].z) -
				(current[2].z - current[0].z)*(current[1].x - current[0].x)
			) / S3DE_STL_AMPLIFICATION,
			(float)(
				(current[1].x - current[0].x)*(current[2].y - current[0].y) -
				(current[1].y - current[0].y)*(current[2].x - current[0].x)
			) / S3DE_STL_AMPLIFICATION
		);
		fprintf(f,"  outer loop\n");
		fprintf(f,"  vertex %f %f %f\n",
			(float)current[0].x / S3DE_STL_AMPLIFICATION,
			(float)current[0].y / S3DE_STL_AMPLIFICATION,
			(float)current[0].z / S3DE_STL_AMPLIFICATION
		);
		fprintf(f,"  vertex %f %f %f\n",
			(float)current[1].x / S3DE_STL_AMPLIFICATION,
			(float)current[1].y / S3DE_STL_AMPLIFICATION,
			(float)current[1].z / S3DE_STL_AMPLIFICATION
		);
		fprintf(f,"  vertex %f %f %f\n",
			(float)current[2].x / S3DE_STL_AMPLIFICATION,
			(float)current[2].y / S3DE_STL_AMPLIFICATION,
			(float)current[2].z / S3DE_STL_AMPLIFICATION
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
	printf("DEBUG > S3DE.c : S3DE_saveSTLfromPlaks() : ");
	printf("Plaks have been exported into \"%s\".\n", fileName);
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
			S3DE_colorBuffer[index  ] = S3DE_SKYCOLOR;
			S3DE_depthBuffer[index++] = FLT_MAX;
		}
	}

	//counter for rendered plaks
	#ifdef DEBUG_ON
	int renderedNbr=0;
	#endif

	//screen rendering
	plak* current = S3DE_plaks;
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
				S3DE_real(current->points[0]).z > S3DE_BACKSCENE_LIMIT ||
				S3DE_real(current->points[1]).z > S3DE_BACKSCENE_LIMIT ||
				S3DE_real(current->points[2]).z > S3DE_BACKSCENE_LIMIT
			)
		){
			S3DE_render(current);

			//count how many plaks are rendered
			#ifdef DEBUG_ON
			renderedNbr++;
			#endif
		}

		//set next plak as current
		current = current->next;
	}

	#ifdef DEBUG_ON
	printf("DEBUG > S3DE.c : S3DE_display() : ");
	printf("%i/%i plaks rendered.\n",renderedNbr,S3DE_plaksNbr);
	#endif

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
static void S3DEL_reshape(int newWidth,int newHeight){
	//error cases
	if(newWidth <= 0 || newHeight <= 0){
		printf("RUNTIME ERROR > S3DE.c : S3DEL_reshape() : ");
		printf("Width and height must be positive numbers.\n");
		return;
	}

	//set new values
	S3DE_width    = newWidth;
	S3DE_width_2  = newWidth/2;
	S3DE_height   = newHeight;
	S3DE_height_2 = newHeight/2;

	//reallocate color & depth buffers
	free(S3DE_colorBuffer);
	free(S3DE_depthBuffer);
	S3DE_colorBuffer = malloc(S3DE_width*S3DE_height*4);
	S3DE_depthBuffer = malloc(S3DE_width*S3DE_height*4);

	//set buffer default values
	int index=0;
	for(int y=0; y < S3DE_height; y++){
		for(int x=0; x < S3DE_width; x++){
			S3DE_colorBuffer[index  ] = S3DE_SKYCOLOR;
			S3DE_depthBuffer[index++] = FLT_MAX;
		}
	}

	//reshape
	glViewport(0,0, newWidth,newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.f,(GLdouble)newWidth, 0.f,(GLdouble)newHeight, -1.f,1.f);
	glMatrixMode(GL_MODELVIEW);
	S3DE_refresh();
	S3DE_reshape(newWidth,newHeight);
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
	return x >= 0 && x < S3DE_width && y >= 0 && y < S3DE_height;
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

void S3DE_quad(float x1,float y1, float x2,float y2, float x3,float y3, float x4,float y4, int filled){
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
			S3DE_colorBuffer[index  ] = S3DE_SKYCOLOR;
			S3DE_depthBuffer[index++] = FLT_MAX;
		}
	}

	#ifdef DEBUG_ON
	//show the number of plaks loaded in scene
	printf("DEBUG > S3DE.c : S3DE_init() : ");
	printf("%i plaks loaded.\n",S3DE_plaksNbr);
	#endif
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
	//free 3D engine memory
	free(S3DE_colorBuffer);
	free(S3DE_depthBuffer);
	for(int p=0; p < S3DE_plaksNbr; p++)
		S3DE_delPlak(S3DE_plaksNbr-1);

	//stop 2D engine
	glutDestroyWindow(S3DE_window);
	exit(EXIT_SUCCESS);
}
