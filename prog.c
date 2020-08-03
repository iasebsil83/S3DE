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

    BUGS : Active bugs in last version
    NOTES : Notes

    Contact : ...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */












// ---------------- DECLARATIONS ----------------

// ---- window ----
const int maxl = 700;	const int maxl_2 = 350;
const int maxh = 700;	const int maxh_2 = 350;

// ---- S3DE shared variables ----
extern xyz S3DE_position;
extern float S3DE_angle[3];

// ---- global vars ----
float angleStep = 0.01;












// ---------------- EXECUTION ----------------

// ---- event handlers ----
void S3DE_timedExecution(){
	S3DE_refresh();
}

void S3DE_display(){
	//blue filled rectangle at the bottom left corner
	S3DE_setColor(  0,  0,255);
	S3DE_rectangle(100,100, 200,200, 1);

	//red unfilled rectangle at the top right corner
	S3DE_setColor(255,  0,  0);
	S3DE_rectangle(maxl-100,maxh-100, maxl-200,maxh-200, 0);
}

void S3DE_keyPressed(char key){
	switch(key){
		case 'z':
			S3DE_angle[0] += angleStep;
		break;
		case 'q':
			S3DE_angle[1] -= angleStep;
		break;
		case 's':
			S3DE_angle[0] -= angleStep;
		break;
		case 'd':
			S3DE_angle[1] += angleStep;
		break;
		case S3DE_KEY_UP:
		break;
		case S3DE_KEY_DOWN:
		break;
		case S3DE_KEY_LEFT:
		break;
		case S3DE_KEY_RIGHT:
		break;
	}
	printf("Pressing key '%c'\n", key);
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
	printf("Mouse is moving passively to (%i,%i)\n", x,y);
}

void S3DE_reshape(int newWidth,int newHeight){
	printf("Reshaping window to %ix%i\n", newWidth,newHeight);
}



// ---- main ----
int main(int argc, char **argv){

	//init S3DE
	S3DE_init(argc,argv, "Program Name [V.V.V]", maxl,maxh);
	S3DE_setTimedExecution(100);

	//create 3D scene
	S3DE_addPlak(
		20,20,20,
		100,100,100,
		50,200,50,
		S3DE_setPixelRGBA(255,0,0, 255)
	);

	//launch S3DE
	printf("Starting S3DE [0.1.0]\n");
	S3DE_start();

	return 0;
}

