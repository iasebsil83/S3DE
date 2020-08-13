// ---------------- IMPORTATIONS ----------------

//standard
#include <stdlib.h>
#include <stdio.h>



//graphics
#include "S3DE.h" //3D motor












/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prog [V.V.V] ~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Developped using S3DE :

    Blablabla about the project.

    DD/MM/YYYY > [V.V.V] :
    - Added something.
    - Added something else.

    DD/MM/YYYY > [V.V.V] :
        - Added something that wasn't before.
        - Fixed a bug.
        - Added something else.
        - Added something else again.

    BUGS : Active bugs in last version.
    NOTES : Notes.

    Contact : ...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */












// ---------------- DECLARATIONS ----------------

// ---- window ----
const int maxl = 700;	const int maxl_2 = 350;
const int maxh = 700;	const int maxh_2 = 350;

// ---- S3DE shared variables ----
extern xyz S3DE_position;
extern float S3DE_FOV;

// ---- global vars ----
float angleStep = 0.1;
int positionStep = 50;
float FOVstep = 0.0001;












// ---------------- EVENTS ----------------

// ---- event handlers ----
void S3DE_timedExecution(){
	//S3DE_refresh();
}

void S3DE_display(){
	//blue filled rectangle at the bottom left corner
	//S3DE_setColor(  0,  0,255);
	//S3DE_rectangle(100,100, 200,200, 1);

	//red unfilled rectangle at the top right corner
	//S3DE_setColor(255,  0,  0);
	//S3DE_rectangle(maxl-100,maxh-100, maxl-200,maxh-200, 0);
}

void S3DE_keyPressed(char key){
	switch(key){
		//rotations
		case 'U':
		case 'u':
			S3DE_addAngleX(angleStep);
		break;
		case 'J':
		case 'j':
			S3DE_addAngleX(-angleStep);
		break;
		case 'I':
		case 'i':
			S3DE_addAngleY(angleStep);
		break;
		case 'K':
		case 'k':
			S3DE_addAngleY(-angleStep);
		break;

		//movements
		case 'Z':
		case 'z':
			S3DE_goStraight(positionStep, S3DE_FRONT);
		break;
		case 'S':
		case 's':
			S3DE_goStraight(positionStep, S3DE_BACK);
		break;
		case 'Q':
		case 'q':
			S3DE_goStraight(positionStep, S3DE_LEFT);
		break;
		case 'D':
		case 'd':
			S3DE_goStraight(positionStep, S3DE_RIGHT);
		break;
		case 'A':
		case 'a':
			S3DE_goStraight(positionStep, S3DE_UP);
		break;
		case 'E':
		case 'e':
			S3DE_goStraight(positionStep, S3DE_DOWN);
		break;

		//get some information
		case ' ':
			printf("Angle : (%f,%f,0.000000), Position : (%f,%f,%f)\n",
				S3DE_getAngleX(), S3DE_getAngleY(),
				S3DE_position.x , S3DE_position.y , S3DE_position.z
			);
		break;

		//save scene
		case 'L':
		case 'l':
			S3DE_saveSTLfromPlaks("STL/save.stl");
		break;

		case 'F':
		case 'f':
			S3DE_fullScreen();
		break;

		//FOV
		case S3DE_KEY_PAGE_UP:
			printf("Increasing FOV from %f to ", S3DE_FOV);
			S3DE_FOV += FOVstep;
			printf("%f.\n", S3DE_FOV);
		break;
		case S3DE_KEY_PAGE_DOWN:
			printf("Decreasing FOV from %f to ", S3DE_FOV);
			S3DE_FOV -= FOVstep;
			printf("%f.\n", S3DE_FOV);
		break;
	}
	S3DE_refresh();
	//printf("Pressing key '%c'\n", key);
}

void S3DE_mousePressed(int x,int y, int state){
	switch(state){
		case S3DE_LEFT_PRESSED:
			printf("Left clicking at (%i,%i)\n", x,y);
		break;
		case S3DE_MIDDLE_PRESSED:
			printf("Middle clicking at (%i,%i)\n", x,y);
		break;
		case S3DE_RIGHT_PRESSED:
			printf("Right clicking at (%i,%i)\n", x,y);
		break;
	}
}

void S3DE_mouseMoved(int x,int y){
	//printf("Mouse is moving passively to (%i,%i)\n", x,y);
}

void S3DE_reshape(int newWidth,int newHeight){
	printf("Reshaping window to %ix%i.\n", newWidth,newHeight);
}












// ---------------- EXECUTION ----------------

// ---- main ----
int main(int argc, char** argv){

	//init S3DE
	S3DE_init(argc,argv, "Program Name [V.V.V]", maxl,maxh);
	S3DE_setTimedExecution(100);

	//create 3D scene
	S3DE_addPlaksFromSTL(
		"STL/dog.stl",
		0,-200,400,
		S3DE_setPixelRGBA(255,0,0,255)
	);
	S3DE_addPlak(
		100,650,200,
		600,650,500,
		300, 50,800,
		S3DE_setPixelRGBA(0,255,0,255)
	);

	//go in an interresting place + look in an interresting direction
	S3DE_addAngleX(1.800000);
	S3DE_addAngleY(6.499996);
	S3DE_position.x = -700;
	S3DE_position.y =    0;
	S3DE_position.z = -850;

	//launch S3DE
	printf("Starting S3DE [0.1.3]\n");
	S3DE_start();

	return EXIT_SUCCESS;
}

