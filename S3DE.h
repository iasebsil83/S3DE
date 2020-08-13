// ---------------- DECLARATIONS ----------------

//special key constants
enum S3DE_KEYBOARD{
	S3DE_KEY_F1        = (char) -1,
	S3DE_KEY_F2        = (char) -2,
	S3DE_KEY_F3        = (char) -3,
	S3DE_KEY_F4        = (char) -4,
	S3DE_KEY_F5        = (char) -5,
	S3DE_KEY_F6        = (char) -6,
	S3DE_KEY_F7        = (char) -7,
	S3DE_KEY_F8        = (char) -8,
	S3DE_KEY_F9        = (char) -9,
	S3DE_KEY_F10       = (char)-10,
	S3DE_KEY_F11       = (char)-11,
	S3DE_KEY_F12       = (char)-12,
	S3DE_KEY_UP        = (char)-13,
	S3DE_KEY_DOWN      = (char)-14,
	S3DE_KEY_LEFT      = (char)-15,
	S3DE_KEY_RIGHT     = (char)-16,
	S3DE_KEY_PAGE_UP   = (char)-17,
	S3DE_KEY_PAGE_DOWN = (char)-18,
	S3DE_KEY_HOME      = (char)-19,
	S3DE_KEY_END       = (char)-20,
	S3DE_KEY_INSERT    = (char)-21,
	S3DE_KEY_BACKSPACE = (char)-22,
	S3DE_KEY_TAB       = (char)-23,
	S3DE_KEY_ENTER     = (char)-24,
	S3DE_KEY_ESCAPE    = (char)-25,
	S3DE_KEY_DELETE    = (char)-26,
	S3DE_KEY_UNDEFINED = (char)-27
};



//mouse constants
enum S3DE_MOUSE{
	S3DE_LEFT_PRESSED    = 0,
	S3DE_LEFT_RELEASED   = 1,
	S3DE_MIDDLE_PRESSED  = 2,
	S3DE_MIDDLE_RELEASED = 3,
	S3DE_RIGHT_PRESSED   = 4,
	S3DE_RIGHT_RELEASED  = 5
};



//3D constants
#define S3DE_RENDER_DISTANCE 1000000000 //in pixels
#define S3DE_SKYCOLOR 16777215          //RGBA color corresponding to (0,255,255,255) (perfect cyan)
#define S3DE_TEXT_FOR_FLOAT 46
#define S3DE_BACKSCENE_LIMIT 0          //max z coordinate for the scene behind you
#define S3DE_FRONT 0                    //    to be rendered (in pixels)
#define S3DE_BACK 1
#define S3DE_LEFT 2
#define S3DE_RIGHT 3
#define S3DE_UP 4
#define S3DE_DOWN 5
#define S3DE_STL_AMPLIFICATION 25       //STL files values will be amplified
                                        //    to correspond to the scene's scale



//3D point
typedef struct{
	float x,y,z;
} xyz;



//3D plaks (linked list)
struct PLAK{
	xyz points[3];
	int color;
	struct PLAK* next;
};
typedef struct PLAK plak;








// ---------------- 3D ENGINE ----------------
float S3DE_getAngleX();
float S3DE_getAngleY();
void S3DE_addAngleX(float angle);
void S3DE_addAngleY(float angle);
void S3DE_goStraight(float distance, char direction);








// ---------------- PLAKS MANAGEMENT ----------------
void S3DE_addPlak(
	int x0,int y0,int z0,
	int x1,int y1,int z1,
	int x2,int y2,int z2,
	int color
);
void S3DE_delPlak(int index);
plak* S3DE_getPlak(int index);








// ---------------- PLAKS / STL FILE CONVERSIONS ----------------
void S3DE_addPlaksFromSTL(char* fileName, int dx,int dy,int dz, int color);
void S3DE_saveSTLfromPlaks(char* fileName);








// ---------------- 2D UTILITIES ----------------

//useful
void S3DE_refresh();
void S3DE_fullScreen();
void S3DE_setColor(int r, int v, int b);
void S3DE_setThickness(float thickness);
int S3DE_inScreen(int x,int y);

//graphics
void S3DE_point(float x, float y);
void S3DE_line(float x1,float y1, float x2,float y2);
void S3DE_triangle(float x1,float y1, float x2,float y2, float x3,float y3, int filled);
void S3DE_rectangle(float x1,float y1, float x2,float y2, int filled);
void S3DE_quad(float x1,float y1, float x2,float y2, float x3,float y3, float x4,float y4, int filled);

//text
void S3DE_text(char* text, float size, float x,float y);

//images
void S3DE_imageRGBA(int x,int y, int width,int height, int* data);
int S3DE_setPixelRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

//timed executions
void S3DE_setTimedExecution(int ms);








// ---------------- BASICS -----------------

//init
void S3DE_init(int argc, char** argv, const char* name, int width,int height);

//start - stop
void S3DE_start();
void S3DE_stop();
