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

// ---- S3DE shared variables ----
extern unsigned int S3DE_width;
extern unsigned int S3DE_height;
extern int S3DE_keyState;
extern unsigned short S3DE_key;
extern int S3DE_mouseX;
extern int S3DE_mouseY;
extern int S3DE_mouseButton;
extern int S3DE_mouseState;
extern xyz S3DE_position;
extern float S3DE_FOV;



// ---- global vars ----
float angleStep = 0.1;
int positionStep = 50;
float FOVstep = 0.0001;



//mobility
#define STATE__IDLE              0
#define STATE__ROTATE_X_PLUS   1
#define STATE__ROTATE_X_MINUS -1
#define STATE__ROTATE_Y_PLUS   2
#define STATE__ROTATE_Y_MINUS -2
#define STATE__GO_FRONT          3
#define STATE__GO_BACK          -3
#define STATE__GO_LEFT           4
#define STATE__GO_RIGHT         -4
#define STATE__GO_UP             5
#define STATE__GO_DOWN          -5
char state = STATE__IDLE;




















// ---------------- 3D SCENE ----------------

//init
void buildScene(){
	//by Beau : "https://www.prusaprinters.org/social/20555-beau/prints"
	S3DE_addPlaksFromSTL(
		"STL/dog.stl",
		0,-200,800,
		S3DE_setPixelRGBA(0,0,0,255)
	);

	//by Vash Wolf : "https://www.myminifactory.com/users/sgtcold1"
	/*
	S3DE_addPlaksFromSTL(
		"STL/minecraft-world.stl",
		0,0,400,
		S3DE_setPixelRGBA(255,0,255,255)
	);
	*/

	//just a simple green plak
	S3DE_addPlak(
		100,650,2500,
		600,650,3000,
		300, 50,2800,
		S3DE_setPixelRGBA(0,255,0,255)
	);
}




















// ---------------- EVENTS ----------------

// ---- event handlers ----
void S3DE_event(int event){
	switch(event){

		case S3DE_DISPLAY:
			//blue filled rectangle at the bottom left corner
			//S3DE_setColor(  0,  0,255);
			//S3DE_rectangle(100,100, 200,200, 1);

			//red unfilled rectangle at the top right corner
			//S3DE_setColor(255,  0,  0);
			//S3DE_rectangle(
			//	S3DE_width-100, S3DE_height-100,
			//	S3DE_width-200, S3DE_height-200,
			//	0
			//);
		break;

		case S3DE_KEYBOARD:

			//"key released" events
			if(S3DE_keyState == S3DE_KEY_RELEASED){
				switch(S3DE_key){

					//rotations
					case S3DE_KEY_U:
					case S3DE_KEY_u:
					case S3DE_KEY_J:
					case S3DE_KEY_j:
					case S3DE_KEY_I:
					case S3DE_KEY_i:
					case S3DE_KEY_K:
					case S3DE_KEY_k:

					//movements
					case S3DE_KEY_Z:
					case S3DE_KEY_z:
					case S3DE_KEY_S:
					case S3DE_KEY_s:
					case S3DE_KEY_Q:
					case S3DE_KEY_q:
					case S3DE_KEY_D:
					case S3DE_KEY_d:
					case S3DE_KEY_A:
					case S3DE_KEY_a:
					case S3DE_KEY_E:
					case S3DE_KEY_e:
						state = STATE__IDLE;
					break;
				}

			//"key pressed" events
			}else{
				switch(S3DE_key){

					//rotations
					case S3DE_KEY_U:
					case S3DE_KEY_u:
						state = STATE__ROTATE_X_PLUS;
					break;
					case S3DE_KEY_J:
					case S3DE_KEY_j:
						state = STATE__ROTATE_X_MINUS;
					break;
					case S3DE_KEY_I:
					case S3DE_KEY_i:
						state = STATE__ROTATE_Y_MINUS;
					break;
					case S3DE_KEY_K:
					case S3DE_KEY_k:
						state = STATE__ROTATE_Y_PLUS;
					break;

					//movements
					case S3DE_KEY_Z:
					case S3DE_KEY_z:
						state = STATE__GO_FRONT;
					break;
					case S3DE_KEY_S:
					case S3DE_KEY_s:
						state = STATE__GO_BACK;
					break;
					case S3DE_KEY_Q:
					case S3DE_KEY_q:
						state = STATE__GO_LEFT;
					break;
					case S3DE_KEY_D:
					case S3DE_KEY_d:
						state = STATE__GO_RIGHT;
					break;
					case S3DE_KEY_A:
					case S3DE_KEY_a:
						state = STATE__GO_UP;
					break;
					case S3DE_KEY_E:
					case S3DE_KEY_e:
						state = STATE__GO_DOWN;
					break;

					//get some information
					case S3DE_KEY_SPACE:
						printf(
							"Angle : (%f,%f,0.000000), Position : (%f,%f,%f)\n",
							S3DE_getAngleX(), S3DE_getAngleY(),
							S3DE_position.x , S3DE_position.y , S3DE_position.z
						);
					break;

					//save scene
					case S3DE_KEY_L:
					case S3DE_KEY_l:
						S3DE_saveSTLfromPlaks("STL/save.stl");
						printf("Saved scene in STL/save.stl\n");
					break;

					//fullScreen
					case S3DE_KEY_F:
					case S3DE_KEY_f:
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

					case S3DE_KEY_ESCAPE:
						S3DE_stop();
						printf("S3DE stopped.\n");
					break;
				}
			}
		break;

		case S3DE_MOUSECLICK:
			if(S3DE_mouseState == S3DE_MOUSE_RELEASED) //get only "mouse pressed" events
				break;

			switch(S3DE_mouseButton){
				case S3DE_LEFT_BUTTON:
					printf("Left clicked at (%i,%i).\n", S3DE_mouseX,S3DE_mouseY);
				break;
				case S3DE_MIDDLE_BUTTON:
					printf("Middle clicked at (%i,%i).\n", S3DE_mouseX,S3DE_mouseY);
				break;
				case S3DE_RIGHT_BUTTON:
					printf("Right clicked at (%i,%i).\n", S3DE_mouseX,S3DE_mouseY);
				break;
			}
		break;

		case S3DE_MOUSEMOVE:
		break;

		case S3DE_TIMER:

			//movement updates
			switch(state){
				case STATE__ROTATE_X_MINUS:
					S3DE_addAngleX(-angleStep);
				break;
				case STATE__ROTATE_X_PLUS:
					S3DE_addAngleX(angleStep);
				break;
				case STATE__ROTATE_Y_MINUS:
					S3DE_addAngleY(-angleStep);
				break;
				case STATE__ROTATE_Y_PLUS:
					S3DE_addAngleY(angleStep);
				break;

				case STATE__GO_FRONT:
					S3DE_goStraight(positionStep, S3DE_FRONT);
				break;
				case STATE__GO_BACK:
					S3DE_goStraight(positionStep, S3DE_BACK);
				break;

				case STATE__GO_LEFT:
					S3DE_goStraight(positionStep, S3DE_LEFT);
				break;
				case STATE__GO_RIGHT:
					S3DE_goStraight(positionStep, S3DE_RIGHT);
				break;

				case STATE__GO_UP:
					S3DE_goStraight(positionStep, S3DE_UP);
				break;
				case STATE__GO_DOWN:
					S3DE_goStraight(positionStep, S3DE_DOWN);
				break;
			}
			S3DE_refresh();
		break;

		case S3DE_RESIZE:
		break;
	}
}




















// ---------------- EXECUTION ----------------

// ---- main ----
int main(int argc, char** argv){

	//init S3DE
	S3DE_init(argc,argv, "Program Name [V.V.V]", 700,700);
	S3DE_setTimer(20); //<=> 50 fps

	//create 3D scene
	buildScene();

	//go in an interresting place + look at an interresting direction
	S3DE_addAngleX(1.800000);
	S3DE_addAngleY(6.499996);
	S3DE_position.x = -200;
	S3DE_position.y =  600;
	S3DE_position.z = -1150;

	//launch S3DE
	printf("Starting S3DE [0.1.6]\n");
	S3DE_start();

	return EXIT_SUCCESS;
}

